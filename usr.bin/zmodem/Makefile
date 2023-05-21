#
# Driver makefile for zmodem.  Calls makefile.generic.
#
TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

# XXX CFLAGS		+= -Werror -DV7 -DMD=2 -DTXBSIZE=4096  -DNFGVMIN -DSEGMENTS=2
CFLAGS		+= -ansi -DV7 -DMD=2 -DTXBSIZE=4096  -DNFGVMIN -DSEGMENTS=2
#CFLAGS		+= -Werror -DV7 -DMD=2 -DTXBSIZE=1024  -DNFGVMIN -DSEGMENTS=8

SRCS		= rz.c sz.c
OBJS		= rz.o sz.o
MAN		    = rz.0 sz.0
MANSRC		= rz.1 sz.1

all:            rz sz $(MAN)

rz:             rz.o
		$(CC) $(LDFLAGS) -o rz.elf rz.o $(LIBS)
		$(OBJDUMP) -S rz.elf > rz.dis
		$(SIZE) rz.elf
		$(ELF2AOUT) rz.elf $@ && rm rz.elf

sz:             sz.o
		$(CC) $(LDFLAGS) -o sz.elf sz.o $(LIBS)
		$(OBJDUMP) -S sz.elf > sz.dis
		$(SIZE) sz.elf
		$(ELF2AOUT) sz.elf $@ && rm sz.elf

rz.0:           rz.1
		$(MANROFF) rz.1 > $@

sz.0:           sz.1
		$(MANROFF) sz.1 > $@

clean:
		rm -f *.o *.elf $(MAN) rz sz *.elf *.dis tags *~

install:        all
		install rz $(DESTDIR)/usr/bin/
		install sz $(DESTDIR)/usr/bin/
		cp rz.0 $(DESTDIR)/usr/share/man/cat1/
		cp sz.0 $(DESTDIR)/usr/share/man/cat1/
		@rm -f $(DESTDIR)/usr/bin/sb $(DESTDIR)/usr/bin/sx
		ln $(DESTDIR)/usr/bin/sz $(DESTDIR)/usr/bin/sb
		ln $(DESTDIR)/usr/bin/sz $(DESTDIR)/usr/bin/sx
		@rm -f $(DESTDIR)/usr/bin/rb $(DESTDIR)/usr/bin/rx
		ln $(DESTDIR)/usr/bin/rz $(DESTDIR)/usr/bin/rb
		ln $(DESTDIR)/usr/bin/rz $(DESTDIR)/usr/bin/rx
		@rm -f $(DESTDIR)/usr/share/man/cat1/sb.0 $(DESTDIR)/usr/share/man/cat1/sx.0
		ln $(DESTDIR)/usr/share/man/cat1/sz.0 $(DESTDIR)/usr/share/man/cat1/sb.0
		ln $(DESTDIR)/usr/share/man/cat1/sz.0 $(DESTDIR)/usr/share/man/cat1/sx.0
		@rm -f $(DESTDIR)/usr/share/man/cat1/rb.0 $(DESTDIR)/usr/share/man/cat1/rx.0
		ln $(DESTDIR)/usr/share/man/cat1/rz.0 $(DESTDIR)/usr/share/man/cat1/rb.0
		ln $(DESTDIR)/usr/share/man/cat1/rz.0 $(DESTDIR)/usr/share/man/cat1/rx.0
