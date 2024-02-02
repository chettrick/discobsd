#
# Version 2.2 January 24, 1998
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall -Os
LDFLAGS		+=

SRCS		= ld.c
OBJS            = ld.o
MAN		= ld.0
MANSRC		= ld.1

all:            ld $(MAN)

ld:             ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o ld.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S ld.elf > ld.dis
		${SIZE} ld.elf
		${ELF2AOUT} ld.elf $@ && rm ld.elf

$(MAN):         $(MANSRC)
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.0 *.elf ${MAN} ld *.elf *.dis a.out tags *~

install:        all
		${INSTALL} ld ${DESTDIR}/usr/bin/ld
		cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
