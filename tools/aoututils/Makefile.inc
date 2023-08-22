TOPSRC!=cd ../../..; pwd

TOOLDIR?=	${TOPSRC}/tools
TOOLBINDIR?=	${TOOLDIR}/bin

HOST_CC?=	cc
CC=		${HOST_CC}

CFLAGS=	-Os -Wall -std=gnu89 -fno-builtin -DCROSS \
	-I. -I../include \
	-idirafter ${TOPSRC}/include \
	-idirafter ../ar

all:	${PROG}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ ${OBJS}

install: all
	install ${PROG} ${TOOLBINDIR}/${PROG}

clean:
	rm -f *.o ${PROG} *~

.PHONY:	all install clean
