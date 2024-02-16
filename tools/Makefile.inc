# Common defines and tools to support cross-hosting platforms.

_HOST_OSNAME!=	uname -s

TOOLDIR?=	${TOPSRC}/tools
TOOLBINDIR?=	${TOOLDIR}/bin

HOST_CC?=	cc

CC=		${HOST_CC}

INSTALL=	install

YACC!=		if [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
			echo "byacc" ; \
		else \
			echo "yacc" ; \
		fi

YFLAGS=		-d

#MANROFF=	nroff -man -h -Tascii
MANROFF=	mandoc -Tascii
