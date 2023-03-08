TOPSRC          = $(shell cd ../..; pwd)
SUBDIR		= startup libc libm libcurses libtermlib libwiznet libreadline libgpanel
PROG		= ar as aout ld nm ranlib size strip

# XXX Add back -Werror to CFLAGS
CFLAGS          = -Os -std=gnu89 -fno-builtin -g -Wall -DCROSS -I. \
                   -idirafter $(TOPSRC)/include \
                   -idirafter $(TOPSRC)/usr.bin/ar \
                   -idirafter $(TOPSRC)/usr.bin/as
LDFLAGS         += -g

AR_OBJS         = ar.o append.o archive.o contents.o delete.o extract.o \
                  misc.o move.o print.o replace.o strmode.o
AOUT_OBJS       = aout.o mips-dis.o # XXX use MACHINE_ARCH
RANLIB_OBJS     = ranlib.o archive.o
HEADERS         = a.out.h ar.h nlist.h ranlib.h

vpath %.c $(TOPSRC)/usr.bin/aout $(TOPSRC)/usr.bin/ar $(TOPSRC)/usr.bin/as \
    $(TOPSRC)/usr.bin/ld $(TOPSRC)/usr.bin/nm $(TOPSRC)/usr.bin/ranlib \
    $(TOPSRC)/usr.bin

all install depend: $(HEADERS) $(SUBDIR) $(PROG)
		-for i in $(SUBDIR); do $(MAKE) -C $$i $(MFLAGS) DESTDIR=$(DESTDIR) $@; done

clean:
		rm -f *~ *.o *.a *.h $(PROG) retroImage gccdump.s
		for i in $(SUBDIR); do $(MAKE) -C $$i $(MFLAGS) clean; done

ar:             $(AR_OBJS)
		$(CC) $(LDFLAGS) -o $@ $(AR_OBJS) $(LIBS)

as:             as.o
		$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

aout:           $(AOUT_OBJS)
		$(CC) $(LDFLAGS) -o $@ $(AOUT_OBJS) $(LIBS)

ld:             ld.o
		$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

nm:             nm.o
		$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

ranlib:         $(RANLIB_OBJS)
		$(CC) $(LDFLAGS) -o $@ $(RANLIB_OBJS) $(LIBS)

size:           size.o
		$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

strip:          strip.o
		$(CC) $(LDFLAGS) -o $@ $< $(LIBS)

$(HEADERS):
		-ln -s -f $(TOPSRC)/include/a.out.h .
		-ln -s -f $(TOPSRC)/include/ar.h .
		-ln -s -f $(TOPSRC)/include/nlist.h .
		-ln -s -f $(TOPSRC)/include/ranlib.h .
