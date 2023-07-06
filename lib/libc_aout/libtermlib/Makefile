TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

LIBDIR=		${TOPSRC}/lib/libtermlib

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS		= -Os ${DEFS} -Wall ${COPTS}

OBJS		= termcap.o tgoto.o tputs.o tcattr.o

all:            ../libtermlib.a

%.o:		${LIBDIR}/%.c
		${CC} ${DEFS} ${CFLAGS} -c $< -o $@

../libtermlib.a: ${OBJS}
		../ar_aout rc $@ ${OBJS}
		../ranlib_aout $@

install:	../libtermlib.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libtermlib.a ${DESTDIR}/usr/lib/libtermlib.a
		../ranlib_aout -t ${DESTDIR}/usr/lib/libtermlib.a

clean:
		rm -f *~ *.o a.out *.a ../libtermlib.a


termcap.o:	${LIBDIR}/termcap.c
tgoto.o:	${LIBDIR}/tgoto.c
tputs.o:	${LIBDIR}/tputs.c
tcattr.o:	${LIBDIR}/tcattr.c
