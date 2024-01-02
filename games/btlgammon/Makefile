#  @(#)Makefile    4.2     (Berkeley)      9/7/85
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS  	= -O -Wall
OBJS    	= btlgammon.o

all:            btlgammon

btlgammon:      ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

clean:
		rm -f *.o *.dis core btlgammon

install:
		install btlgammon ${DESTDIR}/usr/games/
		install -m 644 backrules ${DESTDIR}/usr/games/lib/backrules
