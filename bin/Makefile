#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ..; pwd
include ${TOPSRC}/share/mk/sys.mk

SUBDIR=		date2 hostname ls md5 sh stty test \
		cat chgrp chmod cp dd df echo ed expr kill ln mkdir mv ps \
		pwd rm rmail rmdir sleep sync tar

# TODO: csh

all:		${SUBDIR}

${SUBDIR}:	FRC
		${MAKE} -C $@ ${MFLAGS}

FRC:

install:
		-for i in ${SUBDIR}; do \
			${MAKE} -C $$i ${MFLAGS} DESTDIR=${DESTDIR} install; done

clean:
		/bin/rm -f a.out core *.s *.o *.dis *.elf *~ y.tab.[ch] errs
		for i in ${SUBDIR}; do (cd $$i; ${MAKE} ${MFLAGS} clean); done
