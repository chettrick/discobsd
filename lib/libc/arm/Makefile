#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
# Machine dependent routines for the Arm are located here
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/share/mk/sys.mk

ALL		= gen string sys

arm.a:		${ALL}
		-mkdir tmp
		for i in ${ALL}; do (cd tmp; $(AR) x ../$$i/$$i.a); done
		$(AR) cr arm.a `ls tmp/*.o | sort`
		rm -rf tmp

${ALL}:	FRC
	cd $@; ${MAKE} ${MFLAGS} DEFS="${DEFS}"

FRC:

tags:
		for i in ${ALL}; do \
			(cd $$i; ${MAKE} ${MFLAGS} TAGSFILE=../${TAGSFILE} tags); \
		done

clean:
		for i in ${ALL}; do (cd $$i; ${MAKE} ${MFLAGS} clean); done
		rm -rf *.a tmp *~

depend:
		for i in ${ALL}; do \
			(cd $$i; ${MAKE} ${MFLAGS} DEFS="${DEFS}" depend); done
