TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

LIBDIR=		${TOPSRC}/lib/libreadline

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS		= -Os ${DEFS} -Wall ${COPTS}

OBJS		= readline.o

all:            ../libreadline.a

%.o:		${LIBDIR}/%.c
		${CC} ${DEFS} ${CFLAGS} -c $< -o $@

../libreadline.a: ${OBJS}
		../ar_aout rc $@ ${OBJS}
		../ranlib_aout $@

install:	../libreadline.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libreadline.a ${DESTDIR}/usr/lib/libreadline.a
		../ranlib_aout -t ${DESTDIR}/usr/lib/libreadline.a

clean:
		rm -f *~ *.o a.out *.a ../libreadline.a


readline.o:	${LIBDIR}/readline.c
