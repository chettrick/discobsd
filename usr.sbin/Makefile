#
# Public domain - 1996/10/24 - sms
#
#	@(#)Makefile	1.2 (2.11BSD) 1997/6/27
#
TOPSRC=	$(shell cd ..; pwd)
include	${TOPSRC}/target.mk

SUBDIR=	chroot cron disktool mkpasswd pstat talloc \
	update vipw

# TODO: devupdate

# Programs that must run set-group-id kmem.
#
KMEM=	iostat

BINS=	${KMEM}

all:	${SUBDIR} ${BINS}

${SUBDIR}: FRC
	cd $@; ${MAKE} ${MFLAGS} all

FRC:

# ${KMEM}
%: %.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf $< ${LIBS}
	${OBJDUMP} -S $@.elf > $@.dis
	${SIZE} $@.elf
	${ELF2AOUT} $@.elf $@ && /bin/rm $@.elf

install: FRC
	-for i in ${SUBDIR}; do \
		(cd $$i; ${MAKE} ${MFLAGS} DESTDIR=${DESTDIR} install); done
	-for i in ${KMEM}; do \
		install -m 2751 $$i ${DESTDIR}/bin/$$i; done

clean:
	/bin/rm -f ${BINS} a.out core *.s *.o *.dis *.elf *~ errs
	for i in ${SUBDIR}; do (cd $$i; ${MAKE} ${MFLAGS} clean); done
