#
# Public domain - 1996/10/26 - sms
#
#	@(#)Makefile	1.3 (2.11BSD) 1999/7/21
#
TOPSRC=	$(shell cd ..; pwd)
include	${TOPSRC}/target.mk

SUBDIR=	getty

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; ${MAKE} ${MFLAGS} all

FRC:

install: FRC
	-for i in ${SUBDIR}; do \
		(cd $$i; ${MAKE} ${MFLAGS} DESTDIR=${DESTDIR} install); done

clean:
	-for i in ${SUBDIR}; do (cd $$i; ${MAKE} ${MFLAGS} clean); done
	rm -f *~
