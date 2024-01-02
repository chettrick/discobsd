#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

DEFS    = -DNOSCCS
CFLAGS += -O -DCM_N -DCM_GT -DCM_B -DCM_D ${DEFS} -Wall
LD      = $(CC)
SRCS    = termcap.c tgoto.c tputs.c tcattr.c
OBJS    = termcap.o tgoto.o tputs.o tcattr.o
POBJS   = ../termcap.o ../tgoto.o ../tputs.o ../tcattr.o
TAGSFILE= tags

#.c.o:
#	$(CC) $(CFLAGS) -c -p $*.c
#	$(LD) $(LDFLAGS) -x -r -o profiled/$*.o $*.o
#	$(CC) $(CFLAGS) -c $*.c
#	$(LD) $(LDFLAGS) -X -r $*.o
#	mv a.out $*.o

all:    ../libtermcap.a termcap

.PHONY: termcap

termcap:
	cd termcap && ${MAKE}

../libtermcap.a: ${OBJS}
	$(AR) cr ../libtermcap.a ${OBJS}

../libtermcap_p.a: ${OBJS}
	cd profiled; $(AR) cr ../../libtermcap_p.a ${POBJS}

install: all
#	cp ../libtermcap.a ${DESTDIR}/usr/lib/libtermcap.a
#	@-rm -f ${DESTDIR}/usr/lib/libtermlib.a
#	ln ${DESTDIR}/usr/lib/libtermcap.a ${DESTDIR}/usr/lib/libtermlib.a
#	$(RANLIB) ${DESTDIR}/usr/lib/libtermcap.a
	$(MAKE) -C termcap install
#	cp ../libtermcap_p.a ${DESTDIR}/usr/lib/libtermcap_p.a
#	@-rm -f ${DESTDIR}/usr/lib/libtermlib_p.a
#	ln ${DESTDIR}/usr/lib/libtermcap_p.a ${DESTDIR}/usr/lib/libtermlib_p.a
#	$(RANLIB) ${DESTDIR}/usr/lib/libtermcap_p.a

tags:
	cwd=`pwd`; \
	for i in ${SRCS}; do \
		ctags -a -f ${TAGSFILE} $$cwd/$$i; \
	done

clean:
	-rm -f *.o profiled/*.o
	-rm -f ../libtermcap.a ../libtermcap_p.a
	-rm -f termcap/termcap termcap/termcap.full

VGRIND=	csh /usr/ucb/vgrind
vgrind:
	cp /dev/null index
	${VGRIND} -h "Termcap library" termcap.c tputs.c tgoto.c
	${VGRIND} -h "Termcap library" -x index
