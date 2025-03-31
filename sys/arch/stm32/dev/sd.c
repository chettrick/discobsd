/*
 * SD or SDHC card connected to SPI or SDIO port.
 *
 * Up to two cards can be connected to the same SPI port.
 * PC-compatible partition table is supported.
 * The following device numbers are used:
 *
 * Major Minor Device  Partition
 * ----------------------------------------------
 *   0     0     sd0   Main SD card, whole volume
 *   0     1     sd0a  1-st partition, usually root FS
 *   0     2     sd0b  2-nd partition, usually swap
 *   0     3     sd0c  3-rd partition
 *   0     4     sd0d  4-th partition
 *   0     8     sd1   Second SD card, whole volume
 *   0     9     sd1a  1-st partition
 *   0     10    sd1b  2-nd partition
 *   0     11    sd1c  3-rd partition
 *   0     12    sd1d  4-th partition
 *
 * Copyright (C) 2010-2015 Serge Vakulenko, <serge@vak.ru>
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appear in all
 * copies and that both that the copyright notice and this
 * permission notice and warranty disclaimer appear in supporting
 * documentation, and that the name of the author not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * The author disclaim all warranties with regard to this
 * software, including all implied warranties of merchantability
 * and fitness.  In no event shall the author be liable for any
 * special, indirect or consequential damages or any damages
 * whatsoever resulting from loss of use, data or profits, whether
 * in an action of contract, negligence or other tortious action,
 * arising out of or in connection with the use or performance of
 * this software.
 */
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/errno.h>
#include <sys/dk.h>
#include <sys/disk.h>
#include <sys/kconfig.h>

#include <machine/sd.h>
#include <machine/sdio_card.h>

#define sdunit(dev)     ((minor(dev) & 8) >> 3)
#define sdpart(dev)     ((minor(dev) & 7))
#define RAWPART         0               /* 'x' partition */

/*
 * Detect a card.
 */
static int
sd_setup(int unit)
{
    struct disk *du = &sddrives[unit];
    u_short buf[256];

    if (! card_init(unit)) {
        printf("sd%d: no SD/MMC card detected\n", unit);
        return 0;
    }
    /* Get the size of raw partition. */
    bzero(du->part, sizeof(du->part));
    du->part[RAWPART].dp_offset = 0;
    du->part[RAWPART].dp_nsectors = card_size(unit);
    if (du->part[RAWPART].dp_nsectors == 0) {
        printf("sd%d: cannot get card size\n", unit);
        return 0;
    }

    printf("sd%d: type %s, size %u kbytes\n", unit,
        (du->card_type == TYPE_SDHC) ? "SDHC" :
        (du->card_type == TYPE_SD_II) ? "II" : "I",
        du->part[RAWPART].dp_nsectors / 2);

    /* Read partition table. */
    int s = splbio();
    if (! card_read(unit, 0, (char*)buf, sizeof(buf))) {
        splx(s);
        printf("sd%d: cannot read partition table\n", unit);
        return 0;
    }
    splx(s);
    if (buf[255] == MBR_MAGIC) {
        bcopy(&buf[223], &du->part[1], 64);
        int i;
        for (i=1; i<=NPARTITIONS; i++) {
            if (du->part[i].dp_type != 0)
                printf("sd%d%c: partition type %02x, sector %u, size %u kbytes\n",
                    unit, i+'a'-1, du->part[i].dp_type,
                    du->part[i].dp_offset,
                    du->part[i].dp_nsectors / 2);
        }
    }
    return 1;
}

/*
 * Disable the SD card.
 */
static void
sd_release(int unit)
{
    struct disk *du = &sddrives[unit];

    card_release(unit);

    /* Forget the partition table. */
    du->part[RAWPART].dp_nsectors = 0;
}

/*
 * sd block device
 */

int
sdopen(dev_t dev, int flags, int mode)
{
    int unit = sdunit(dev);
    int part = sdpart(dev);
    struct disk *du = &sddrives[unit];
    unsigned mask, i;

    if (unit >= NSD || part > NPARTITIONS)
        return ENXIO;

    /*
     * Setup the SD card interface.
     */
    if (du->part[RAWPART].dp_nsectors == 0) {
        if (! sd_setup(unit)) {
            return ENODEV;
        }
    }
    mask = 1 << part;

    /*
     * Warn if a partion is opened
     * that overlaps another partition which is open
     * unless one is the "raw" partition (whole disk).
     */
    if (part != RAWPART && (du->openpart & mask) == 0) {
        unsigned start = du->part[part].dp_offset;
        unsigned end = start + du->part[part].dp_nsectors;

        /* Check for overlapped partitions. */
        for (i=0; i<=NPARTITIONS; i++) {
            struct diskpart *pp = &du->part[i];

            if (i == part || i == RAWPART)
                continue;

            if (pp->dp_offset + pp->dp_nsectors <= start ||
                pp->dp_offset >= end)
                continue;

            if (du->openpart & (1 << i))
                printf("sd%d%c: overlaps open partition (sd%d%c)\n",
                    unit, part + 'a' - 1,
                    unit, pp - du->part + 'a' - 1);
        }
    }
    du->openpart |= mask;
    return 0;
}

int
sdclose(dev_t dev, int mode, int flag)
{
    int unit = sdunit(dev);
    int part = sdpart(dev);
    struct disk *du = &sddrives[unit];

    if (unit >= NSD || part > NPARTITIONS)
        return ENODEV;

    du->openpart &= ~(1 << part);
    if (du->openpart == 0) {
        /* All partitions closed.
         * Release the SD card. */
        sd_release(unit);
    }
    return 0;
}

/*
 * Get disk size in kbytes.
 * Return nonzero if successful.
 */
daddr_t
sdsize(dev_t dev)
{
    int unit = sdunit(dev);
    int part = sdpart(dev);
    struct disk *du = &sddrives[unit];

    if (unit >= NSD || part > NPARTITIONS || du->openpart == 0)
        return 0;

    return du->part[part].dp_nsectors >> 1;
}

void
sdstrategy(struct buf *bp)
{
    int unit = sdunit(bp->b_dev);
    struct disk *du = &sddrives[unit];
    struct diskpart *p = &du->part[sdpart(bp->b_dev)];
    int part_size = p->dp_nsectors >> 1;
    int offset = bp->b_blkno;
    long nblk = btod(bp->b_bcount);
    int s;

    /*
     * Determine the size of the transfer, and make sure it is
     * within the boundaries of the partition.
     */
    offset += p->dp_offset >> 1;
    if (offset == 0 &&
        ! (bp->b_flags & B_READ) && ! du->label_writable)
    {
        /* Write to partition table not allowed. */
        bp->b_error = EROFS;
bad:    bp->b_flags |= B_ERROR;
        biodone(bp);
        return;
    }
    if (bp->b_blkno + nblk > part_size) {
        /* if exactly at end of partition, return an EOF */
        if (bp->b_blkno == part_size) {
            bp->b_resid = bp->b_bcount;
            biodone(bp);
            return;
        }
        /* or truncate if part of it fits */
        nblk = part_size - bp->b_blkno;
        if (nblk <= 0) {
            bp->b_error = EINVAL;
            goto bad;
        }
        bp->b_bcount = nblk << DEV_BSHIFT;
    }

    if (bp->b_dev == swapdev) {
        led_control(LED_SWAP, 1);
    } else {
        led_control(LED_DISK, 1);
    }

    s = splbio();
#ifdef UCB_METER
    if (du->dkindex >= 0) {
        dk_busy |= 1 << du->dkindex;
        dk_xfer[du->dkindex]++;
        dk_bytes[du->dkindex] += bp->b_bcount;
    }
#endif

    if (bp->b_flags & B_READ) {
        card_read(unit, offset, bp->b_addr, bp->b_bcount);
    } else {
        card_write(unit, offset, bp->b_addr, bp->b_bcount);
    }

    biodone(bp);
    if (bp->b_dev == swapdev) {
        led_control(LED_SWAP, 0);
    } else {
        led_control(LED_DISK, 0);
    }
#ifdef UCB_METER
    if (du->dkindex >= 0)
        dk_busy &= ~(1 << du->dkindex);
#endif
    splx(s);
}

int
sdioctl(dev_t dev, u_int cmd, caddr_t addr, int flag)
{
    int unit = sdunit(dev);
    int part = sdpart(dev);
    struct diskpart *dp;
    int i, error = 0;

    switch (cmd) {

    case DIOCGETMEDIASIZE:
        /* Get disk size in kbytes. */
        dp = &sddrives[unit].part[part];
        *(int*) addr = dp->dp_nsectors >> 1;
        break;

    case DIOCREINIT:
        for (i=0; i<=NPARTITIONS; i++)
            bflush(makedev(major(dev), i));
        sd_setup(unit);
        break;

    case DIOCGETPART:
        /* Get partition table entry. */
        dp = &sddrives[unit].part[part];
        *(struct diskpart*) addr = *dp;
        break;

    default:
        error = EINVAL;
        break;
    }
    return error;
}

/*
 * Test to see if device is present.
 * Return true if found and initialized ok.
 */
static int
sd_probe(config)
    struct conf_device *config;
{
    int unit = config->dev_unit;
    const char *ctlr_name = config->dev_cdriver->d_name;
    int ctlr_num = config->dev_ctlr;

    if (unit < 0 || unit >= NSD)
        return 0;
    printf("sd%u: port %s%d\n", unit, ctlr_name, ctlr_num);

    if (! card_init(unit)) {
        printf("sd%u: cannot open %s%u port\n", unit, ctlr_name, ctlr_num);
        return 0;
    }

    /* Disable the SD card. */
    sd_release(unit);

#ifdef UCB_METER
    dk_alloc(&sddrives[unit].dkindex, 1, (unit == 0) ? "sd0" : "sd1");
#endif
    return 1;
}

struct driver sddriver = {
    "sd", sd_probe,
};
