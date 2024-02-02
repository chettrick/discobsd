#	@(#)Makefile	8.1 (Berkeley) 6/5/93

FILES=	words

all clean cleandir depend lint tags:

install:
	${INSTALL} -d ${DESTDIR}/usr/share/dict
	${INSTALL} -c -m 444 ${FILES} ${DESTDIR}/usr/share/dict
