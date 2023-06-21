#
# Public domain - 1996/10/24 - sms
#
#	@(#)Makefile	1.2 (2.11BSD) 1997/6/27
#
TOPSRC!=cd ..; pwd
include	${TOPSRC}/target.mk

SUBDIR=	chroot cron disktool iostat mkpasswd pstat talloc \
	update vipw

# TODO: devupdate

all:	${SUBDIR}

${SUBDIR}: FRC
	cd $@; ${MAKE} ${MFLAGS} all

FRC:

install: FRC
	-for i in ${SUBDIR}; do \
		(cd $$i; ${MAKE} ${MFLAGS} DESTDIR=${DESTDIR} install); done

clean:
	/bin/rm -f a.out core *.s *.o *.dis *.elf *~ errs
	for i in ${SUBDIR}; do (cd $$i; ${MAKE} ${MFLAGS} clean); done
