#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	@(#)Makefile	5.1.2	(2.11BSD)	1999/10/25
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

CFLAGS          += -Os -Wall -ansi
CFLAGS          += -DBSD -DDEST=\"/usr/games/lib/atc/\"
#CFLAGS          += -DSYSV -DDEST=\"games/\"
LEX             = lex
LIBS            = -lm -lcurses -ltermcap -lc
OBJS            = extern.o grammar.o input.o lex.o list.o log.o main.o tunable.o \
                  graphics.o update.o
MAN		= atc.0
MANSRC		= atc.6

all:            atc $(MAN)

atc:            $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o atc.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S atc.elf > atc.dis
		$(SIZE) atc.elf
		$(ELF2AOUT) atc.elf $@ && rm atc.elf

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.0 *.elf atc ${MAN} *.elf *.dis tags *~ grammar.c lex.c y.tab.[ch]

install:        all
		install atc ${DESTDIR}/usr/games/
		mkdir -p ${DESTDIR}/usr/games/lib/atc
		cp -p games/* ${DESTDIR}/usr/games/lib/atc
		cp ${MAN} ${DESTDIR}/usr/share/man/cat6/
