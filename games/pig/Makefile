#	@(#)Makefile	4.2	(Berkeley)	9/7/85
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

OBJS            = pig.o
MAN             = pig.0
MANSRC          = pig.6

all:            pig $(MAN)

pig:           $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o core pig *.0 *.dis

install:        all
		${INSTALL} pig ${DESTDIR}/usr/games/
		${INSTALL} -m 644 ${MAN} ${DESTDIR}/usr/share/man/cat6/
