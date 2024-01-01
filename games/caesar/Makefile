#	@(#)Makefile	4.2	(Berkeley)	9/7/85
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

#CFLAGS  	+= -Wall

LIBS		= -lm -lc

OBJS            = caesar.o
MAN             = caesar.0
MANSRC          = caesar.6

all:            caesar $(MAN)

caesar:           $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && rm $@.elf

${MAN}:		${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o core caesar *.0 *.dis

install:        all
		install caesar $(DESTDIR)/usr/games/
		install -m 644 $(MAN) $(DESTDIR)/usr/share/man/cat6/
