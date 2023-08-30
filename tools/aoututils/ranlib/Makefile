#
# ranlib - generate index to archive
#

PROG=	ranlib
OBJS=	archive.o ranlib.o

include	../Makefile.inc

CFLAGS+=-Wno-format

archive.o: ../ar/archive.c
	${CC} ${CFLAGS} -c -o $@ ../ar/archive.c
