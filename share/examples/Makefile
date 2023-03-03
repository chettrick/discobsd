# Public Domain.  2023/02/21 - Christopher Hettrick

all clean cleandir depend lint tags:

install:
	install -d ${DESTDIR}/share/examples
	tar cf - . | (cd ${DESTDIR}/share/examples && tar xpf -)
