#
# Public Domain.  1995/03/13 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

BIN		= msec
SRCS            = $(BIN).c
OBJS            = $(BIN).o
MAN             = $(BIN).0

all:            $(BIN) ${MAN}

$(BIN):    ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

.SUFFIXES:      .0 .1

.1.0:
		${MANROFF} $*.1 > $@

clean:
		rm -f *.o *.elf ${MAN} *.elf *.dis tags *~ $(BIN)

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		${INSTALL} -m 755 ${BIN} ${DESTDIR}/usr/bin

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}
