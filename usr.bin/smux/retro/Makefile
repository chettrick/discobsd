#
# Public Domain.  1995/03/13 - Steven Schultz
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Wall -I${TOPSRC}/sys/include

BIN		= smux
SRCS            = smux.c
OBJS            = smux.o

all: smux

smux: $(OBJS)
			${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
			${OBJDUMP} -S $@.elf > $@.dis
			${SIZE} $@.elf
			${ELF2AOUT} $@.elf $@

clean:
		rm -f *.o *.elf *.elf *.dis tags *~ $(BIN)

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		${INSTALL} -m 755 ${BIN} ${DESTDIR}/usr/bin

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}
