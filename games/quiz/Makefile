#	@(#)Makefile	4.2	(Berkeley)	9/7/85
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk
#include $(TOPSRC)/cross.mk
#CFLAGS  	= -O -DCROSS -Werror -Wall

CFLAGS  	+= -Werror -Wall

OBJS            = quiz.o
MAN             = quiz.0

all:            quiz $(MAN)

quiz:           $(OBJS)
		$(CC) $(LDFLAGS) -o $@.elf $(OBJS) $(LIBS)
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

$(MAN):         quiz.6
		nroff -man $< > $@

clean:
		rm -f *.o core quiz *.0 *.dis

install:        all
		install quiz $(DESTDIR)/usr/games/
		install -m 644 $(MAN) $(DESTDIR)/usr/share/man/cat6/
		cp -r quiz.k ${DESTDIR}/usr/games/lib
