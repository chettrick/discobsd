TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

PROG=	xinstall
OBJS=	xinstall.o

# Use uninstalled copy of the install program.
INSTALL=./xinstall

# libbsd-dev package on Linux in overlay mode.
_LIBBSD_CFLAGS!=if [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
		pkg-config libbsd-overlay --cflags ; \
	else \
		echo "" ; \
	fi

_LIBBSD_LIBS!=if [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
		pkg-config libbsd-overlay --libs ; \
	else \
		echo "" ; \
	fi

CFLAGS+=${_LIBBSD_CFLAGS}
LIBS+=	${_LIBBSD_LIBS}

all:	${PROG}

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

install: all
	${INSTALL} ${PROG} ${TOOLBINDIR}/binstall

clean:
	rm -f *~ *.o ${PROG}

.PHONY: all install clean
