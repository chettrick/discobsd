#	@(#)Makefile	4.2	(Berkeley)	9/7/85
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk
#include $(TOPSRC)/cross.mk
#CFLAGS  	= -O -DCROSS -Wall

#CFLAGS  	+= -Wall

LIBS		= -lm -lc

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
		install pig $(DESTDIR)/usr/games/
		install -m 644 $(MAN) $(DESTDIR)/usr/share/man/cat6/
