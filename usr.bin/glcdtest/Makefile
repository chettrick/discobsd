#
# Public Domain.  1995/03/13 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -I${TOPSRC}/sys/include

SRCS            = glcdtest.c
OBJS            = glcdtest.o

all:            glcdtest

glcdtest:    ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o glcdtest.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S glcdtest.elf > glcdtest.dis
		${SIZE} glcdtest.elf
		${ELF2AOUT} glcdtest.elf $@

.SUFFIXES:      .0 .1

clean:
		rm -f *.o *.elf *.elf *.dis tags *~ glcdtest

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		install -m 755 glcdtest ${DESTDIR}/usr/bin

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}
