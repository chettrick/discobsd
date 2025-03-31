/*
 * SPI driver for STM32.
 */
#include <sys/param.h>
#include <sys/uio.h>
#include <sys/kconfig.h>
#include <machine/spi.h>

int
spi_open(dev_t dev, int flag, int mode)
{
	return (0);
}

int
spi_close(dev_t dev, int flag, int mode)
{
	return (0);
}

int
spi_read(dev_t dev, struct uio *uio, int flag)
{
	return (0);
}

int
spi_write(dev_t dev, struct uio *uio, int flag)
{
	return (0);
}

int
spi_ioctl(dev_t dev, u_int cmd, caddr_t addr, int flag)
{
	return (0);
}

static int
spi_probe(struct conf_ctlr *config)
{
	return (1);
}

struct driver spidriver = {
	"spi", spi_probe,
};
