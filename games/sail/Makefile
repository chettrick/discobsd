#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Os -Wall

OBJS            = main.o pl_main.o pl_1.o pl_2.o pl_3.o pl_4.o pl_5.o pl_6.o pl_7.o \
                  dr_main.o dr_1.o dr_2.o dr_3.o dr_4.o dr_5.o lo_main.o \
                  assorted.o game.o globals.o misc.o parties.o sync.o version.o doprnt.o
LIBS            = -lcurses -ltermcap -lc
MAN		= sail.0
MANSRC		= sail.6

all:            sail $(MAN)

sail:           ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o sail.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S sail.elf > sail.dis
		${SIZE} sail.elf
		${ELF2AOUT} sail.elf $@ && rm sail.elf

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.0 *.elf ${MAN} sail *.elf *.dis tags *~

install:        all
		${INSTALL} sail ${DESTDIR}/usr/games/
		cp ${MAN} ${DESTDIR}/usr/share/man/cat6/
