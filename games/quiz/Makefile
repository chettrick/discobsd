#	@(#)Makefile	4.2	(Berkeley)	9/7/85
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS  	+= -Wall

OBJS            = quiz.o
MAN             = quiz.0
MANSRC          = quiz.6

all:            quiz $(MAN)

quiz:           $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o core quiz *.0 *.dis

install:        all
		${INSTALL} quiz ${DESTDIR}/usr/games/
		${INSTALL} -m 644 ${MAN} ${DESTDIR}/usr/share/man/cat6/
		cp -r quiz.k ${DESTDIR}/usr/games/lib
