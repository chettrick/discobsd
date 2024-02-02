# Public Domain.  2023/02/21 - Christopher Hettrick

all clean cleandir depend lint tags:

install:
	${INSTALL} -d ${DESTDIR}/usr/share/examples
	tar cf - . | (cd ${DESTDIR}/usr/share/examples && tar xpf -)
