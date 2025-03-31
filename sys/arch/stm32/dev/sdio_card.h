#ifndef _MACHINE_SDIO_CARD_H_
#define _MACHINE_SDIO_CARD_H_

#ifdef KERNEL
int     card_init(int unit);
int     card_size(int unit);
int     card_read(int unit, unsigned int offset, char *data, unsigned int bcount);
int     card_write(int unit, unsigned offset, char *data, unsigned bcount);
void    card_release(int unit);
#endif /* KERNEL */

#endif /* _MACHINE_SDIO_CARD_H_ */
