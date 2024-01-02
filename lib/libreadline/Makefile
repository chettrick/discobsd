TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS          += -O -Wall

OBJS		= readline.o
HDRS		= readline/readline.h readline/history.h

all:            ../libreadline.a

../libreadline.a: ${OBJS}
		@${AR} cr $@ ${OBJS}
		$(RANLIB) $@

install: 	all ${HDRS}
		install -d ${DESTDIR}/usr/include/readline/
		cp -p ${HDRS} ${DESTDIR}/usr/include/readline/

clean:
		rm -f *~ *.o a.out ../libreadline*.a
