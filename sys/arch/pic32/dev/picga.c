/*
 * PICGA driver for PIC32.
 *
 * Copyright (C) 2012 Majenko Technologies <matt@majenko.co.uk>
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
#include <sys/conf.h>
#include <sys/user.h>
#include <sys/ioctl.h>
#include <sys/systm.h>
#include <sys/uio.h>
#include <sys/picga.h>
#include <sys/spi.h>

#include <machine/debug.h>

extern int uwritec(struct uio *);

struct spiio picga_io;

void
blockdelay(u_int v)
{
	for (; v > 0; v--)
		asm volatile("nop");
}

void
picga_command(u_char cmd, u_char len, void *data)
{
	struct spiio *io = &picga_io;

	char *p = (char *)data;

	spi_select(io);
	blockdelay(3000);
	spi_transfer(io, cmd);
	blockdelay(3000);
	spi_transfer(io, len);
	blockdelay(3000);
	spi_transfer(io, 0);
	blockdelay(3000);

	while (len > 0) {
		spi_transfer(io, *p++);
		blockdelay(3000);
		len--;
	}

	blockdelay(3000);
	spi_deselect(io);
	blockdelay(3000);
}

int
picga_open(dev_t dev, int flag, int mode)
{
	int channel;
	struct spiio *io = &picga_io;
	struct intval i;
	struct coord2 j;

	channel = minor(dev);
	if (channel > 0)
		return ENODEV;

	if (spi_setup(io, PICGA_BUS, (u_int *)&PICGA_CS_PORT, PICGA_CS_PIN) != 0)
		return ENODEV;

	spi_brg(io, 500);

	i.value = 0x01;
	picga_command(SPI_FGCOLOR, sizeof(struct intval), &i);
	i.value = 0x00;
	picga_command(SPI_BGCOLOR, sizeof(struct intval), &i);
	i.value = FONT_TOPAZ;
	picga_command(SPI_FONT, sizeof(struct intval), &i);
	j.x = 0;
	j.y = 0;
	picga_command(SPI_CLUT, sizeof(struct coord2), &j);
	j.x = 1;
	j.y = 0xFFFF;
	picga_command(SPI_CLUT, sizeof(struct coord2), &j);

	return 0;
}

int
picga_close(dev_t dev, int flag, int mode)
{
	int channel;

	channel = minor(dev);
	if (channel > 0)
		return ENODEV;

	return 0;
}

/* Return the most recent ADC value. */
int
picga_read(dev_t dev, struct uio *uio, int flag)
{
	return EPERM;
}

/* Trigger an ADC conversion. */
int
picga_write(dev_t dev, struct uio *uio, int flag)
{
	int channel;
	char t[100];
	int p = 0;
	char c;

	channel = minor(dev);
	if (channel > 0)
		return ENODEV;

	while (uio->uio_iov->iov_len > 0) {
		p = 0;
		while (p < 90 && uio->uio_iov->iov_len > 0) {
			c = uwritec(uio);
			t[p++] = c;
			t[p] = 0;
			if (c == '\n')
				break;
		}
		picga_command(SPI_PRINT, p, t);
		blockdelay(50000);
	}

	return 0;
}

int
picga_ioctl(dev_t dev, u_int cmd, caddr_t addr, int flag)
{
	printf("IOCTL %08X\n", cmd);

	switch (cmd) {
	case PICGA_CLS:
		printf("CLS command\n");
		picga_command(SPI_CLS, 0, NULL);
		break;
	default:
		return EINVAL;
	}

	return 0;
}
