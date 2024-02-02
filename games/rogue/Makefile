#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that this notice is preserved and that due credit is given
# to the University of California at Berkeley. The name of the University
# may not be used to endorse or promote products derived from this
# software without specific written prior permission. This software
# is provided ``as is'' without express or implied warranty.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Os -DUNIX -DUNIX_BSD4_2 -DCURSES -DANSI -Wall -Wno-format-truncation

OBJS            = curses.o	\
                  doprnt.o	\
                  hit.o		\
                  init.o	\
                  inventory.o	\
                  level.o	\
                  machdep.o	\
                  main.o	\
                  message.o	\
                  monster.o	\
                  move.o	\
                  object.o	\
                  pack.o	\
                  play.o	\
                  random.o	\
                  ring.o	\
                  room.o	\
                  save.o	\
                  score.o	\
                  spec_hit.o	\
                  throw.o	\
                  trap.o	\
                  use.o		\
                  zap.o
MAN		= rogue.0
MANSRC		= rogue.6

all:            rogue $(MAN)

rogue:          ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o rogue.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S rogue.elf > rogue.dis
		${SIZE} rogue.elf
		${ELF2AOUT} rogue.elf $@ && rm rogue.elf

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.0 *.elf ${MAN} rogue *.elf *.dis tags *~

install:        all
		${INSTALL} rogue ${DESTDIR}/usr/games/
		cp ${MAN} ${DESTDIR}/usr/share/man/cat6/

#$(OBJS):        rogue.h
