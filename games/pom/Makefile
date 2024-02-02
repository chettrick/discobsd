#	@(#)Makefile	4.2	(Berkeley)	9/7/85
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

#CFLAGS  	+= -Wall

LIBS		= -lm -lc

OBJS            = pom.o
MAN             = pom.0
MANSRC          = pom.6

all:            pom $(MAN)

pom:           $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o core pom *.0 *.dis

install:        all
		${INSTALL} pom ${DESTDIR}/usr/games/
		${INSTALL} -m 644 ${MAN} ${DESTDIR}/usr/share/man/cat6/
