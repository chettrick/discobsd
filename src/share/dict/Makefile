#	@(#)Makefile	8.1 (Berkeley) 6/5/93

FILES=	words

all clean cleandir depend lint tags:

install:
	install -d ${DESTDIR}/share/dict
	install -c -m 444 ${FILES} ${DESTDIR}/share/dict
