TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS          += -Wall

SRCS		= init.c
OBJS		= init.o
MAN		= init.0
MANSRC		= init.8

all:		init init.0

init:		${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o init.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S init.elf > init.dis
		${SIZE} init.elf
		${ELF2AOUT} init.elf $@ && rm init.elf

init.0:		${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *~ *.elf *.dis ${MAN} init tags

depend:		${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:	all
		cp ${MAN} ${DESTDIR}/usr/share/man/cat8/
		${INSTALL} -m 700 init ${DESTDIR}/sbin/init

lint:		${SRCS}
		lint -hax ${SRCS}

tags:		${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
