/*
 * Copyright (c) 2021 Christopher Hettrick <chris@structfoo.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/param.h>
#include <sys/systm.h>

#include <machine/sd.h>
#include <machine/sdio_card.h>

#ifdef SPI_SD_ENABLED
#include <machine/stm32_adafruit_sd.h>
#endif /* SPI_SD_ENABLED */

#ifdef SDIO_ENABLED
#include <machine/stm32469i_discovery_sd.h>
#endif /* SDIO_ENABLED */

#include <machine/debug.h>

#define SECTSIZE        512

extern struct disk sddrives[NSD];

/*
 * Initialize a card.
 * Return nonzero if successful.
 */
int
card_init(int unit)
{
    struct disk *du = &sddrives[unit];
    BSP_SD_CardInfo ci;

    if (BSP_SD_Init() != MSD_OK) {
        return 0;
    }

    BSP_SD_GetCardInfo(&ci);
#if 0 // XXX SD CARD TYPE
    du->card_type = ci.CardType;
#endif // XXX SD CARD TYPE
    du->card_type = TYPE_SDHC; // XXX SD CARD TYPE

    return 1;
}

/*
 * Get disk size in 512-byte sectors.
 * Return nonzero if successful.
 */
int
card_size(int unit)
{
    BSP_SD_CardInfo ci;

    BSP_SD_GetCardInfo(&ci);

    return ci.LogBlockNbr;
}

/*
 * Read a block of data.
 * Return nonzero if successful.
 */
int
card_read(int unit, unsigned int offset, char *data, unsigned int bcount)
{
    int nblocks;
    uint8_t SD_state = MSD_OK;

    if ((bcount % SECTSIZE) == 0) {
        nblocks = bcount / SECTSIZE;
    } else {
        nblocks = (bcount / SECTSIZE) + 1;
    }

    DEBUG("card_read:  bcount: %d\tnblocks: %d\tbcount \% %d: %d\n",
      bcount, nblocks, SECTSIZE, bcount % SECTSIZE);

    SD_state = BSP_SD_ReadBlocks((uint32_t *)data, offset << 1, nblocks, SD_DATATIMEOUT);

    if (SD_state != MSD_OK) {
        printf("card_read:  read failed\n");
        return 0;
    }

    return 1;
}

/*
 * Write a block of data.
 * Return nonzero if successful.
 */
int
card_write(int unit, unsigned offset, char *data, unsigned bcount)
{
    int nblocks;
    uint8_t SD_state = MSD_OK;

    if ((bcount % SECTSIZE) == 0) {
        nblocks = bcount / SECTSIZE;
    } else {
        nblocks = (bcount / SECTSIZE) + 1;
    }

    DEBUG("card_write: bcount: %d\tnblocks: %d\tbcount \% %d: %d\n",
      bcount, nblocks, SECTSIZE, bcount % SECTSIZE);

    SD_state = BSP_SD_WriteBlocks((uint32_t *)data, offset << 1, nblocks, SD_DATATIMEOUT);

    /* Wait for write completion. */
    int x = spl0();
    while ((BSP_SD_GetCardState() != MSD_OK))
        ;
    splx(x);

    if (SD_state != MSD_OK) {
        printf("card_write: write failed\n");
        return 0;
    }

    return 1;
}

/*
 * Disable the SD card.
 */
void
card_release(int unit)
{
    BSP_SD_DeInit();
}
