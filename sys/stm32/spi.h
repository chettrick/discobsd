#ifndef _MACHINE_SPI_H_
#define _MACHINE_SPI_H_

#ifdef KERNEL
int	spi_open(dev_t dev, int flag, int mode);
int	spi_close(dev_t dev, int flag, int mode);
int	spi_read(dev_t dev, struct uio *uio, int flag);
int	spi_write(dev_t dev, struct uio *uio, int flag);
int	spi_ioctl(dev_t dev, u_int cmd, caddr_t addr, int flag);
#endif /* KERNEL */

#endif /* _MACHINE_SPI_H_ */
