#
# Nifty foo monopoly maker
#	Program:  Ken Arnold
#	Makefile:  K.S.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall
LIBS            = -lc

OBJS            = monop.o cards.o execute.o getinp.o houses.o jail.o misc.o \
                  morg.o print.o prop.o rent.o roll.o spec.o trade.o
MAN		= monop.0
MANSRC		= monop.6

all:            monop $(MAN) cards.pck

monop:          $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o monop.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S monop.elf > monop.dis
		$(SIZE) monop.elf
		$(ELF2AOUT) monop.elf $@ && rm monop.elf

initdeck:       initdeck.c deck.h
		${HOST_CC} -o initdeck initdeck.c

cards.pck:      #initdeck cards.inp
		./initdeck

$(MAN):         $(MANSRC)
		$(MANROFF) $(MANSRC) > $@

clean:
		rm -f *.o *.0 *.elf $(MAN) monop initdeck *.elf *.dis tags *~

install:        all
		${INSTALL} monop ${DESTDIR}/usr/games/
		cp $(MAN) $(DESTDIR)/usr/share/man/cat6/
		cp cards.pck $(DESTDIR)/usr/games/lib/
###
cards.o: cards.c extern.h monop.h deck.h
execute.o: execute.c extern.h monop.h deck.h
getinp.o: getinp.c
houses.o: houses.c extern.h monop.h deck.h
initdeck.o: initdeck.c deck.h
jail.o: jail.c extern.h monop.h deck.h
misc.o: misc.c extern.h monop.h deck.h
monop.o: monop.c defines.h monop.h deck.h mon.dat.h prop.dat.h brd.dat.h
morg.o: morg.c extern.h monop.h deck.h
print.o: print.c extern.h monop.h deck.h
prop.o: prop.c extern.h monop.h deck.h
rent.o: rent.c extern.h monop.h deck.h
roll.o: roll.c
spec.o: spec.c extern.h monop.h deck.h
strcmp.o: strcmp.c
trade.o: trade.c extern.h monop.h deck.h
