TOPSRC		= $(shell cd ../..; pwd)
include $(TOPSRC)/target.mk

PROG		= arch
OBJS		= ${PROG}.o
MAN		= ${PROG}.0
MANSRC		= ${PROG}.1

CFLAGS		+= -Werror -Wall

all:		${PROG} ${MAN}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} $< > $@

clean:
		rm -f *.o *.elf ${MAN} ${PROG} *.dis tags *~

install:	all
		install ${PROG} ${DESTDIR}/bin/
		install ${PROG} ${DESTDIR}/bin/machine
		cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
