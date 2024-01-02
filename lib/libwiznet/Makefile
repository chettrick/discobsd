TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS          += -O -Wall

OBJS    	= w5100.o socket.o ethernet.o client.o server.o udp.o

all:    	../libwiznet.a

../libwiznet.a: ${OBJS}
		@${AR} cr $@ ${OBJS}
		$(RANLIB) $@

install: 	all

clean:
		rm -f *.o a.out core test errs ../libwiznet*.a
