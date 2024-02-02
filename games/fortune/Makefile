TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -O -Wall
LIBS            = -lc

OBJS            = fortune.o
MAN		= fortune.0
MANSRC		= fortune.6

all:            fortune $(MAN) fortunes.dat

fortune:        $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o fortune.elf ${OBJS} ${LIBS}
		$(OBJDUMP) -S fortune.elf > fortune.dis
		$(SIZE) fortune.elf
		$(ELF2AOUT) fortune.elf $@ && rm fortune.elf

strfile:	strfile.c
		${HOST_CC} -o strfile strfile.c

unstr:		unstr.c
		${HOST_CC} -ansi -o unstr unstr.c

fortunes.dat:   #fortunes strfile
		./strfile -r fortunes

fortunes:       scene obscene
		cp scene fortunes
		echo "%-" >> fortunes
		cat obscene >> fortunes

$(MAN):         $(MANSRC)
		$(MANROFF) $(MANSRC) > $@

clean:
		rm -f *.o *.0 *.elf $(MAN) fortune strfile unstr fortunes *.elf *.dis tags *~

install:        all
		${INSTALL} fortune ${DESTDIR}/usr/games/
		cp $(MAN) $(DESTDIR)/usr/share/man/cat6/
		cp fortunes.dat $(DESTDIR)/usr/games/lib/

#$(OBJS):        fortune.h
