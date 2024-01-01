#
# aout    - Display information from a.out files
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

CFLAGS          += -Wall -Os
LDFLAGS         +=

AOUTOBJS        = aout.o mips-dis.o

all:            aout

aout:           $(AOUTOBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o aout.elf ${AOUTOBJS} ${LIBS}
		${OBJDUMP} -S aout.elf > aout.dis
		${SIZE} aout.elf
		${ELF2AOUT} aout.elf $@ && rm aout.elf

clean:
		rm -f *.o *.0 *.elf aout tags *~ *.dis

install:        all
		install aout ${DESTDIR}/usr/bin/
