#ifndef _SD_H
#define _SD_H

#include <sys/disk.h>

/*
 * Driver's data per disk drive.
 */
struct disk {
    /*
     * Partition table.
     */
#define NPARTITIONS     4
    struct  diskpart part[NPARTITIONS+1];

#define TYPE_UNKNOWN    0
#define TYPE_SD_LEGACY  1
#define TYPE_SD_II      2
#define TYPE_SDHC       3
    int     card_type;
    int     label_writable;     /* is sector 0 writable? */
    int     dkindex;            /* disk index for statistics */
    u_int   openpart;           /* all partitions open on this drive */
    u_char  ocr[4];             /* operation condition register */
    u_char  csd[16];            /* card-specific data */
    u_short group[6];           /* function group bitmasks */
    int     ma;                 /* power consumption */
};

struct disk sddrives[NSD];      /* Table of units */

#ifdef KERNEL
extern int      sdopen(dev_t dev, int flag, int mode);
extern int      sdclose(dev_t dev, int flag, int mode);
extern void     sdstrategy(struct buf *bp);
extern daddr_t  sdsize(dev_t dev);
extern int      sdioctl(dev_t dev, u_int cmd, caddr_t addr, int flag);
#endif /* KERNEL */

#endif /* _SD_H */
