#
# Public Domain.  1/15/1995 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

# XXX CFLAGS		+= -Werror

SRCS		= sysctl.c
OBJS		= sysctl.o
MAN             = sysctl.0
MANSRC          = sysctl.8

all:            sysctl $(MAN)

sysctl:      	${OBJS}
		${CC} ${LDFLAGS} -o sysctl.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S sysctl.elf > sysctl.dis
		${SIZE} sysctl.elf
		${ELF2AOUT} sysctl.elf $@ && rm sysctl.elf

$(MAN):         ${MANSRC}
		${MANROFF} $< > $@

clean:
		rm -f *.o *.elf ${MAN} sysctl *.elf *.dis tags *~

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		install sysctl ${DESTDIR}/sbin/sysctl
		cp ${MAN} $(DESTDIR)/usr/share/man/cat8/

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.
