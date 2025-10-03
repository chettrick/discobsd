#ifndef _PICGA_H
#define _PICGA_H

#define SPI_IDLE        0x00

/* System control */
#define SPI_CLS         0x01
#define SPI_CLUT        0x02
#define SPI_ENCOPPER    0x03
#define SPI_DISCOPPER   0x04
#define SPI_COPPERFILL  0x05
#define SPI_SCROLL      0x06
#define SPI_COPY        0x07

/* Basic drawing */
#define SPI_PLOT        0x11
#define SPI_DRAW        0x12
#define SPI_COLOR       0x13
#define SPI_RECTANGLE   0x14
#define SPI_CIRCLE      0x15

/* Text commands */
#define SPI_LOCATE      0x80
#define SPI_FONT        0x81
#define SPI_PRINT       0x82
#define SPI_PRINTAT     0x83
#define SPI_PUTCH       0x84
#define SPI_PUTCHAR     0x85
#define SPI_FGCOLOR     0x86
#define SPI_BGCOLOR     0x87

#define FONT_SINCLAIR   0x00
#define FONT_TOPAZ      0x01

#define UP              0
#define DOWN            1
#define LEFT            2
#define RIGHT           3

struct coord2 {
	u_short x;
	u_short y;
} __attribute__((packed));

struct coord4 {
	u_short x1;
	u_short y1;
	u_short x2;
	u_short y2;
} __attribute__((packed));

struct coord6 {
	u_short x1;
	u_short y1;
	u_short x2;
	u_short y2;
	u_short x3;
	u_short y3;
} __attribute__((packed));

struct intval {
	u_short value;
} __attribute__((packed));

struct rectangle {
	u_short x1;
	u_short y1;
	u_short x2;
	u_short y2;
	u_int fill:1;
	u_char dither;
} __attribute__((packed));

struct circle {
	u_short x;
	u_short y;
	u_short radius;
	u_int fill:1;
} __attribute__((packed));

struct charval {
	u_char value;
} __attribute__((packed));

#ifdef KERNEL
#include "conf.h"

extern void picga_command(u_char cmd, u_char len, void *data);
extern int picga_open(dev_t dev, int flag, int mode);
extern int picga_close(dev_t dev, int flag, int mode);
extern int picga_read(dev_t dev, struct uio *uio, int flag);
extern int picga_write(dev_t dev, struct uio *uio, int flag);
extern int picga_ioctl(dev_t dev, u_int cmd, caddr_t addr, int flag);
#endif

#define PICGA_CLS       _IO('g', 1)

#endif
