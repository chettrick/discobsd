TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

LIBDIR=		${TOPSRC}/lib/libwiznet

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS		= -Os ${DEFS} -Wall ${COPTS}

OBJS		= w5100.o socket.o ethernet.o client.o server.o udp.o

all:            ../libwiznet.a

%.o:		${LIBDIR}/%.c
		${CC} ${DEFS} ${CFLAGS} -c $< -o $@

../libwiznet.a:	${OBJS}
		../ar_aout rc $@ ${OBJS}
		../ranlib_aout $@

install:	../libwiznet.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libwiznet.a ${DESTDIR}/usr/lib/libwiznet.a
		../ranlib_aout -t ${DESTDIR}/usr/lib/libwiznet.a

clean:
		rm -f *~ *.o a.out *.a ../libwiznet.a


w5100.o:	${LIBDIR}/w5100.c
socket.o:	${LIBDIR}/socket.c
ethernet.o:	${LIBDIR}/ethernet.c
client.o:	${LIBDIR}/client.c
server.o:	${LIBDIR}/server.c
udp.o:		${LIBDIR}/udp.c
