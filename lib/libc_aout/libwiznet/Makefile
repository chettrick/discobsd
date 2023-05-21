TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/lib/libwiznet

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS		= -Os ${DEFS} -Wall -Werror ${COPTS}

OBJS		= w5100.o socket.o ethernet.o client.o server.o udp.o

all:            ../libwiznet.a

../libwiznet.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libwiznet.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libwiznet.a ${DESTDIR}/usr/lib/libwiznet.a

clean:
		rm -f *~ *.o a.out *.a
