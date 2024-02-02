TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall

SRCS		= portio.c
OBJS		= portio.o

all:            portio lol

portio:         ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o portio.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S portio.elf > portio.dis
		${SIZE} portio.elf
		${ELF2AOUT} portio.elf $@ && rm portio.elf

lol:            lol.o
		${CC} ${CFLAGS} ${LDFLAGS} -o lol.elf lol.o ${LIBS}
		${OBJDUMP} -S lol.elf > lol.dis
		${SIZE} lol.elf
		${ELF2AOUT} lol.elf $@ && rm lol.elf

clean:
		rm -f *.o *.elf portio lol *.elf *.dis *~

install:        all
		${INSTALL} portio ${DESTDIR}/usr/bin/
		${INSTALL} lol ${DESTDIR}/usr/bin/
