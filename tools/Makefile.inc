# Common defines and tools to support cross-hosting platforms.

_HOST_OSNAME!=	uname -s

TOOLDIR?=	${TOPSRC}/tools
TOOLBINDIR?=	${TOOLDIR}/bin

HOST_CC?=	cc

CC=		${HOST_CC}

# libbsd-dev package on Linux in overlay mode.
_LIBBSD_CFLAGS!=if [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
			pkg-config libbsd-overlay --cflags ; \
		else \
			echo "" ; \
		fi

_LIBBSD_LIBS!=	if [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
			pkg-config libbsd-overlay --libs ; \
		else \
			echo "" ; \
		fi

INSTALL=	${TOOLBINDIR}/binstall -U

YACC!=		if [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
			echo "byacc" ; \
		else \
			echo "yacc" ; \
		fi

YFLAGS=		-d

#MANROFF=	nroff -man -h -Tascii
MANROFF=	mandoc -Tascii
