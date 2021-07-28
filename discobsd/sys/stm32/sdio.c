/*
 * SDIO driver for STM32.
 */
#include <sys/param.h>
#include <sys/uio.h>
#include <sys/kconfig.h>
#include <machine/sdio.h>

int
sdio_open(dev_t dev, int flag, int mode)
{
    return (0);
}

int
sdio_close(dev_t dev, int flag, int mode)
{
    return (0);
}

int
sdio_read(dev_t dev, struct uio *uio, int flag)
{
    return (0);
}

int
sdio_write(dev_t dev, struct uio *uio, int flag)
{
    return (0);
}

int
sdio_ioctl(dev_t dev, u_int cmd, caddr_t addr, int flag)
{
    return (0);
}

static int
sdio_probe(struct conf_ctlr *config)
{
    return (1);
}

struct driver sdiodriver = {
    "sdio", sdio_probe,
};
