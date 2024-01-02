#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	@(#)Makefile	5.3 (Berkeley) 9/7/85
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS  	+= -O -Wall
OBJS    	= boggle.o
MAN             = boggle.0
MANSRC          = boggle.6

all:            boggle bogdict ${MAN}

boggle:         ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

bogdict:        comp sfile
		sed -f sfile ${TOPSRC}/share/dict/words | uniq | ./comp > bogdict

comp:           comp.c
		${HOST_CC} -O comp.c -o comp

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.0 *.dis core boggle comp bogdict

install:
		install boggle ${DESTDIR}/usr/games/
		install -m 644 bogdict ${DESTDIR}/usr/games/lib/bogdict
		cp ${MAN} ${DESTDIR}/usr/share/man/cat6/
