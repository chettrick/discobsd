#
# Copyright (c) 1983 Regents of the University of California,
# All rights reserved.  Redistribution permitted subject to
# the terms of the Berkeley Software License Agreement.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

# libbsd-dev package on Linux in overlay mode.
HOST_CFLAGS+=	${_LIBBSD_CFLAGS}
HOST_LIBS+=	${_LIBBSD_LIBS}

CFLAGS		+= -Os -DUNIX_BSD4_2 -DANSI -DEXT_MESSAGE_FILE -Wall

OBJS            = battlestar.o cmd1.o cmd2.o cmd3.o cmd4.o cmd5.o cmd6.o cmd7.o \
		  init.o cypher.o getcom.o parse.o room.o save.o fly.o misc.o \
		  _globals.o _dayfile.o _nightfile.o dayobjs.o nightobjs.o words.o \
		  curses.o doprnt.o
LIBS            = -lc
MAN		= battlestar.0
MANSRC		= battlestar.6

all:            battle_strings battlestar $(MAN)

battlestar:     ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o battlestar.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S battlestar.elf > battlestar.dis
		${SIZE} battlestar.elf
		${ELF2AOUT} battlestar.elf $@ && rm battlestar.elf

battle_strings _globals.c _dayfile.c _nightfile.c: mkstr globals.c dayfile.c nightfile.c
		./mkstr battle_strings _ globals.c dayfile.c nightfile.c

mkstr:          mkstr.c
		${HOST_CC} -O ${HOST_CFLAGS} mkstr.c -o $@ ${HOST_LIBS}

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.0 *.elf ${MAN} battlestar mkstr *.elf *.dis tags *~ battle_strings _globals.c _dayfile.c _nightfile.c

install:        all
		${INSTALL} battlestar ${DESTDIR}/usr/games/
		cp battle_strings ${DESTDIR}/usr/games/lib/
		cp ${MAN} ${DESTDIR}/usr/share/man/cat6/
