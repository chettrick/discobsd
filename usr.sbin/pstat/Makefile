#
# Should be installed with kmem group.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

SRCS            = pstat.c
OBJS            = pstat.o
MAN             = pstat.0
MANSRC          = pstat.8

all:            pstat ${MAN}

pstat:          ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o pstat.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S pstat.elf > pstat.dis
		${SIZE} pstat.elf
		${ELF2AOUT} pstat.elf $@ && rm pstat.elf

${MAN}:         ${MANSRC}
		${MANROFF} ${MANSRC} > ${MAN}

clean:
		rm -f *.o *.elf ${MAN} pstat *.elf *.dis tags *~

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        pstat
		cp ${MAN} ${DESTDIR}/usr/share/man/cat8/
		install -m 2755 pstat ${DESTDIR}/usr/sbin/pstat

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
