#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -O -Wall #-DV7
LIBS            += -ltermcap -lc

OBJS            = allow.o board.o check.o extra.o fancy.o init.o main.o \
                  move.o odds.o one.o save.o subs.o table.o text.o message.o
TOBJS           = allow.o board.o check.o data.o fancy.o init.o odds.o one.o \
                  save.o subs.o table.o teach.o ttext1.o ttext2.o tutor.o
MAN		= backgammon.0
MANSRC		= backgammon.6

all:            backgammon teachgammon $(MAN)

backgammon:     $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o backgammon.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S backgammon.elf > backgammon.dis
		$(SIZE) backgammon.elf
		$(ELF2AOUT) backgammon.elf $@ && rm backgammon.elf

teachgammon:    $(TOBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o teachgammon.elf ${TOBJS} ${LIBS}
		$(OBJDUMP) -S teachgammon.elf > teachgammon.dis
		$(SIZE) teachgammon.elf
		$(ELF2AOUT) teachgammon.elf $@ && rm teachgammon.elf

$(MAN):         $(MANSRC)
		$(MANROFF) $(MANSRC) > $@

clean:
		rm -f *.o *.0 *.elf $(MAN) backgammon teachgammon *.elf *.dis tags *~

install:        all
		${INSTALL} backgammon ${DESTDIR}/usr/games/
		${INSTALL} teachgammon ${DESTDIR}/usr/games/
		cp $(MAN) $(DESTDIR)/usr/share/man/cat6/

$(OBJS) $(TOBJS): back.h
data.o tutor.o: tutor.h
