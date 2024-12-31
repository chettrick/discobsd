#
# Copyright (c) 1988 Regents of the University of California.
# All rights reserved.
#
# Redistribution and use in source and binary forms are permitted
# provided that the above copyright notice and this paragraph are
# duplicated in all such forms and that any documentation, advertising
# materials, and other materials related to such redistribution and
# use acknowledge that the software was developed by the University
# of California, Berkeley.  The name of the University may not be
# used to endorse or promote products derived from this software
# without specific prior written permission.  THIS SOFTWARE IS PROVIDED
# ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
# WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF MERCHANTIBILITY AND
# FITNESS FOR A PARTICULAR PURPOSE.
#
TOPSRC!=cd ../../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS          += ${DEFS}

OBJS=		addsf3.o comparesf2.o divsf3.o fixsfsi.o fixunssfsi.o \
		floatsisf.o floatunsisf.o mulsf3.o negsf2.o subsf3.o \
		adddf3.o comparedf2.o divdf3.o fixdfsi.o fixunsdfsi.o \
		floatsidf.o floatunsidf.o muldf3.o negdf2.o subdf3.o \
		ashldi3.o lshrdi3.o \
		sc_case.o

all:		${OBJS}

clean:
		rm -f *.a *.o *~ profiled/*.o tags

cleandir:       clean
		rm -f .depend

install:        all
#		cp ../libgcc.a ${DESTDIR}/usr/lib/
#		$(RANLIB) -t ${DESTDIR}/usr/lib/libgcc.a
#		cp libgcc_p.a ${DESTDIR}/usr/lib/
#		$(RANLIB) -t ${DESTDIR}/usr/lib/libgcc_p.a
