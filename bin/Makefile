#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC		= $(shell cd ..; pwd)
include ${TOPSRC}/target.mk

# XXX CFLAGS+=	-Wall

YFLAGS=

# Programs that live in subdirectories, and have makefiles of their own.
# /bin
SUBDIR=		date2 hostname ls md5 sh stty test

# TODO: csh

# Shell scripts that need only be installed and are never removed.
#
SCRIPT=

# C programs that live in the current directory and do not need
# explicit make lines.
#
STD=		cat chgrp chmod cp dd echo ed kill ln mkdir mv \
		pwd rm rmail rmdir sleep sync tar

# C programs that live in the current directory and need explicit make lines.
#
NSTD=		expr

# Programs that must run setuid to root
#
SETUID=

# Programs that run set-group-id operator
#
OPERATOR=	df

# Programs that must run set-group-id kmem.
#
KMEM=		ps

# Programs that must run set-group-id tty.
#
TTY=

BINS=		${STD} ${NSTD} ${SETUID} ${OPERATOR} ${KMEM} ${TTY}

all:		${SUBDIR} ${BINS}

${SUBDIR}:	FRC
		${MAKE} -C $@ ${MFLAGS}

FRC:

# ${STD} ${SETUID} ${KMEM} ${OPERATOR} ${TTY}
%: %.c
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf $< ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && /bin/rm $@.elf

# Files listed in ${NSTD} have explicit make lines given below.

# expr
%.c: %.y
		${YACC} ${YFLAGS} $<
		-/bin/mv y.tab.c $@

install:	${BINS}
		install ${STD} ${NSTD} ${DESTDIR}/bin/
		-for i in ${SUBDIR}; do \
			${MAKE} -C $$i ${MFLAGS} DESTDIR=${DESTDIR} install; done
		-for i in ${SCRIPT}; do (install -c $$i.sh ${DESTDIR}/bin/$$i); done
		-for i in ${SETUID}; do \
			install -m 4751 $$i ${DESTDIR}/bin/$$i; done
		-for i in ${OPERATOR}; do \
			install -m 2751 $$i ${DESTDIR}/bin/$$i; done
		-for i in ${KMEM}; do \
			install -m 2751 $$i ${DESTDIR}/bin/$$i; done
		-for i in ${TTY}; do \
			install -m 2751 $$i ${DESTDIR}/bin/$$i; done

clean:
		/bin/rm -f ${BINS} expr.c a.out core *.s *.o *.dis *.elf *~ y.tab.[ch] errs
		for i in ${SUBDIR}; do (cd $$i; ${MAKE} ${MFLAGS} clean); done

depend:		expr.c
		for i in ${BINS}; do \
		    cc -M ${INCPATH} $$i.c | sed 's/\.o//' | \
		    awk ' { if ($$1 != prev) \
			{ if (rec != "") print rec; rec = $$0; prev = $$1; } \
			else { if (length(rec $$2) > 78) { print rec; rec = $$0; } \
			else rec = rec " " $$2 } } \
			END { print rec } ' >> makedep; done
		echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
		echo '$$r makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile Makefile.bak
		ed - Makefile < eddep
		/bin/rm eddep makedep
		echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
		echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
		echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it
