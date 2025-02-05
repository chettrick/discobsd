TOPSRC!=cd ../../..; pwd

-include ${TOPSRC}/tools/Makefile.inc

CFLAGS=	-Os -Wall -std=gnu89 -fno-builtin -DCROSS \
	-I. -I../include \
	-idirafter ${TOPSRC}/include \
	-idirafter ../ar

# libbsd-dev package on Linux in overlay mode.
CFLAGS+=${_LIBBSD_CFLAGS}
LIBS+=	${_LIBBSD_LIBS}

all:	${PROG}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

install: all
	${INSTALL} ${PROG} ${TOOLBINDIR}/${PROG}

clean:
	rm -f *.o ${PROG} *~

.PHONY:	all install clean
