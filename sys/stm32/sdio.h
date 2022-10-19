#ifndef _MACHINE_SDIO_H_
#define _MACHINE_SDIO_H_

#ifdef KERNEL
int sdio_open(dev_t dev, int flag, int mode);
int sdio_close(dev_t dev, int flag, int mode);
int sdio_read(dev_t dev, struct uio *uio, int flag);
int sdio_write(dev_t dev, struct uio *uio, int flag);
int sdio_ioctl(dev_t dev, u_int cmd, caddr_t addr, int flag);
#endif /* KERNEL */

#endif /* _MACHINE_SDIO_H_ */
