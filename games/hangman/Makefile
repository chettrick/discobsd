TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS  	+= -Wall

OBJS            = endgame.o extern.o getguess.o getword.o main.o playgame.o \
		  prdata.o prman.o prword.o setup.o
MAN             = hangman.0
MANSRC          = hangman.6
LIBS            = -lcurses -ltermcap -lc

all:            hangman $(MAN)

hangman:       $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o core hangman *.0 *.dis

install:        all
		${INSTALL} hangman ${DESTDIR}/usr/games/
		${INSTALL} -m 644 ${MAN} ${DESTDIR}/usr/share/man/cat6/
