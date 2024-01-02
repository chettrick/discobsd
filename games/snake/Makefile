#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	@(#)Makefile	5.1.1 (2.11BSD GTE) 1/16/95
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS  	+= -Wall

OBJS            = snake.o move.o
MAN             = snake.0
MANSRC          = snake.6
LIBS            = -lm -ltermcap -lc

all:            snake snscore $(MAN)

snake:          $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

snscore:        snscore.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf snscore.o ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o core snake snscore *.0 *.dis

install:        all
		install snake ${DESTDIR}/usr/games/
		install snscore ${DESTDIR}/usr/games/
		install -m 644 ${MAN} ${DESTDIR}/usr/share/man/cat6/
		ln -f ${DESTDIR}/usr/share/man/cat6/${MAN} ${DESTDIR}/usr/share/man/cat6/snscore.0
		install -c -m 644 /dev/null ${DESTDIR}/usr/games/lib/snakerawscores
