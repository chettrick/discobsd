TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk
#include $(TOPSRC)/cross.mk

CFLAGS		+= -Os -Wall

OBJS            = done.o init.o io.o main.o save.o subr.o vocab.o wizard.o
MAN		= adventure.0
MANSRC		= adventure.6

all:            adventure $(MAN) adventure.dat

# Data file format is compatible for all little-endian machines.
# To rebuild it, compile the program on Linux.
adventure.dat:  #glorkz
		./adventure glorkz

adventure:      ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o adventure.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S adventure.elf > adventure.dis
		${SIZE} adventure.elf
		${ELF2AOUT} adventure.elf $@ && rm adventure.elf

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.0 *.elf ${MAN} adventure *.elf *.dis tags *~

install:        all
		install adventure ${DESTDIR}/usr/games/
		cp adventure.dat ${DESTDIR}/usr/games/lib/
		cp ${MAN} ${DESTDIR}/usr/share/man/cat6/

$(OBJS):        hdr.h
