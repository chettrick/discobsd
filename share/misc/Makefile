#
# Public domain - 1996/10/21 - sms
#
#  Makefile for misc data files
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

SRCS		=
# eign Mail.help Mail.tildehelp units gprof.callg gprof.flat \
# vlpmacs mkproto.data

all:		${SRCS}

install:	${SRCS}
		-mkdir -p ${DESTDIR}/usr/share/misc
		-chmod a+r,a+x ${DESTDIR}/usr/share/misc
#		${INSTALL} -c -m 444 ${SRCS} ${DESTDIR}/usr/share/misc

clean:
		rm -f *~
