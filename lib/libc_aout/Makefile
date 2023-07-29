TOPSRC!=cd ../..; pwd

HOST_CC?=	cc

CC=		${HOST_CC}

SUBDIR		= startup libc libm libcurses libtermlib libwiznet libreadline libgpanel

PROG		= ar_aout as_aout aout_aout ld_aout nm_aout ranlib_aout size_aout strip_aout

CFLAGS          = -Os -std=gnu89 -fno-builtin -Wall -DCROSS -I. \
                   -idirafter $(TOPSRC)/include \
                   -idirafter $(TOPSRC)/usr.bin/ar \
                   -idirafter $(TOPSRC)/usr.bin/as
LDFLAGS         += -g

AR_OBJS         = ar.o append.o archive.o contents.o delete.o extract.o \
                  misc.o move.o print.o replace.o strmode.o
AOUT_OBJS       = aout.o mips-dis.o
RANLIB_OBJS     = ranlib.o archive.o
HEADERS         = a.out.h ar.h nlist.h ranlib.h

D		= ${TOPSRC}/usr.bin

VPATH		= $D/ar $D/as $D/aout $D/ld $D/nm $D/ranlib $D/size $D/strip

all install depend: $(HEADERS) $(SUBDIR) $(PROG)
		-for i in $(SUBDIR); do $(MAKE) -C $$i $(MFLAGS) DESTDIR=$(DESTDIR) $@; done

clean:
		rm -f *~ *.o *.a *.h $(PROG) retroImage gccdump.s
		for i in $(SUBDIR); do $(MAKE) -C $$i $(MFLAGS) clean; done


ar_aout:	$(AR_OBJS)
		$(CC) $(LDFLAGS) -o $@ $(AR_OBJS) $(LIBS)

as_aout:	as.o
		$(CC) $(LDFLAGS) -o $@ as.o $(LIBS)

aout_aout:	$(AOUT_OBJS)
		$(CC) $(LDFLAGS) -o $@ $(AOUT_OBJS) $(LIBS)

ld_aout:	ld.o
		$(CC) $(LDFLAGS) -o $@ ld.o $(LIBS)

nm_aout:	nm.o
		$(CC) $(LDFLAGS) -o $@ nm.o $(LIBS)

ranlib_aout:	$(RANLIB_OBJS)
		$(CC) $(LDFLAGS) -o $@ $(RANLIB_OBJS) $(LIBS)

size_aout:	size.o
		$(CC) $(LDFLAGS) -o $@ size.o $(LIBS)

strip_aout:	strip.o
		$(CC) $(LDFLAGS) -o $@ strip.o $(LIBS)

$(HEADERS):
		-ln -s -f $(TOPSRC)/include/a.out.h .
		-ln -s -f $(TOPSRC)/include/ar.h .
		-ln -s -f $(TOPSRC)/include/nlist.h .
		-ln -s -f $(TOPSRC)/include/ranlib.h .


ar.o:		$D/ar/ar.c
append.o:	$D/ar/append.c
archive.o:	$D/ar/archive.c
contents.o:	$D/ar/contents.c
delete.o:	$D/ar/delete.c
extract.o:	$D/ar/extract.c
misc.o:		$D/ar/misc.c
move.o:		$D/ar/move.c
print.o:	$D/ar/print.c
replace.o:	$D/ar/replace.c
strmode.o:	$D/ar/strmode.c

as.o:		$D/as/as.c

aout.o:		$D/aout/aout.c
mips-dis.o:	$D/aout/mips-dis.c

ld.o:		$D/ld/ld.c

nm.o:		$D/nm/nm.c

ranlib.o:	$D/ranlib/ranlib.c

size.o:		$D/size/size.c

strip.o:	$D/strip/strip.c
