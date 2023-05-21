TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/lib/libreadline

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS		= -Os ${DEFS} -Wall -Werror ${COPTS}

OBJS		= readline.o

all:            ../libreadline.a

../libreadline.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libreadline.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libreadline.a ${DESTDIR}/usr/lib/libreadline.a

clean:
		rm -f *~ *.o a.out *.a
