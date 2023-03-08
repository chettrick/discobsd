TOPSRC		= $(shell cd ../../..; pwd)
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/lib/libwiznet

CFLAGS		= -Os $(DEFS) -Wall -Werror

ifeq ($(MACHINE_ARCH), mips)
CFLAGS		+= -Wa,-x -B$(TOPSRC)/lib/libc_aout/
endif

OBJS		= w5100.o socket.o ethernet.o client.o server.o udp.o

all:            ../libwiznet.a

../libwiznet.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libwiznet.a
		${INSTALLDIR} ${DESTDIR}/lib
		${INSTALL} ../libwiznet.a ${DESTDIR}/lib/libwiznet.a

clean:
		rm -f *~ *.o a.out *.a
