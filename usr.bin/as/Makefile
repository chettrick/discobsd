#
# as      - Assembler
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall -Os
LDFLAGS         +=

all:            as

as:             as.o
		${CC} ${CFLAGS} ${LDFLAGS} -o as.elf as.o ${LIBS}
		${OBJDUMP} -S as.elf > as.dis
		${SIZE} as.elf
		${ELF2AOUT} as.elf $@ && rm as.elf

clean:
		rm -f *.o *.0 *.elf as tags *~ *.dis tests/*.dis tests/*.gcc-dis tests/*.o

install:        all
		${INSTALL} as ${DESTDIR}/usr/bin/
