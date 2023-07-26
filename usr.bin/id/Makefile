#
# 1997/6/25 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

SRCS		= id.c
OBJS		= id.o
MAN		= id.0 groups.0 whoami.0

all:            id ${MAN}
		install -c groups.sh groups
		install -c whoami.sh whoami

id:             ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o id.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S id.elf > id.dis
		${SIZE} id.elf
		${ELF2AOUT} id.elf $@ && rm id.elf

.SUFFIXES:      .0 .1

.1.0:
		${MANROFF} $*.1 > $@

clean:
		rm -f *.o *.elf ${MAN} id groups whoami *.elf *.dis tags *~

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
		install id ${DESTDIR}/usr/bin/id
		install groups.sh ${DESTDIR}/usr/bin/groups
		install whoami.sh ${DESTDIR}/usr/bin/whoami

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
