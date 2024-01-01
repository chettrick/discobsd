#
# mille bourne game makefile
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

CFLAGS  	+= -Wall

OBJS            = comp.o end.o extern.o init.o mille.o misc.o move.o print.o \
                  roll.o save.o types.o varpush.o
MAN             = mille.0
MANSRC          = mille.6
LIBS            = -lcurses -ltermcap -lc

all:            mille $(MAN)

mille:          $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o core mille *.0 *.dis

install:        all
		install mille $(DESTDIR)/usr/games/
		install -m 644 $(MAN) $(DESTDIR)/usr/share/man/cat6/
