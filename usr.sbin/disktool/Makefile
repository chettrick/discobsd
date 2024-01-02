TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall

SRCS		= disktool.c
OBJS		= disktool.o

all:            disktool

disktool:       ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o disktool.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S disktool.elf > disktool.dis
		${SIZE} disktool.elf
		${ELF2AOUT} disktool.elf $@ && rm disktool.elf

clean:
		rm -f *.o *.elf disktool *.elf *.dis *~

install:        all
		install disktool ${DESTDIR}/usr/sbin/
