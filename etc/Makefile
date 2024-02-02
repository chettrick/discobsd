#
# Public domain - 1997/1/10 - sms
#
#	@(#)Makefile	1.0 (2.11BSD) 1997/1/10
#
# DESTDIR is defined here to be something which does *NOT* exist - it must be
#  specified on the command line when doing a "make DESTDIR=/mnt distribution".
#  This is aimed at avoiding overwriting the system disk's /etc files.
#
# Example:
#	mount /dev/ra1a /mnt
#	make DESTDIR=/mnt distribution

DESTDIR=/foobar
OWN=	root
GRP=	wheel

TZDIR=		/usr/share/zoneinfo
LOCALTIME=	US/Pacific

FILES=	fstab gettytab group motd passwd rc rc.local shells ttys

all install depend lint tags:

clean:
	rm -f *~

distribution: distribution-etc-root-var

distribution-etc-root-var: distrib-dirs
	${INSTALL} -d -o ${OWN} -g ${GRP} -m 755 ${DESTDIR}/dev
	${INSTALL} -d -o ${OWN} -g ${GRP} -m 755 ${DESTDIR}/etc

	${INSTALL} -c -o ${OWN} -g ${GRP} -m 644 ${FILES} ${DESTDIR}/etc
	${INSTALL} -c -o root -g wheel -m 600 shadow ${DESTDIR}/etc
#	${INSTALL} -c -o root -g wheel -m 600 master.passwd ${DESTDIR}/etc
#	${INSTALL} -c -o ${OWN} -g ${GRP} -m 555 MAKEDEV.local MAKEDEV \
#	    ${DESTDIR}/dev
	${INSTALL} -c -o ${OWN} -g ${GRP} -m 555 MAKEDEV \
	    ${DESTDIR}/etc
	cd root; \
		${INSTALL} -c -o root -g wheel -m 644 dot.profile \
		    ${DESTDIR}/root/.profile; \
		${INSTALL} -c -o root -g wheel -m 644 dot.profile \
		    ${DESTDIR}/.profile
	ln -sf ${TZDIR}/${LOCALTIME} ${DESTDIR}/etc/localtime
	${INSTALL} -c -m 444 ${DESTDIR}/usr/share/misc/termcap \
	    ${DESTDIR}/etc/termcap
#	${INSTALL} -c -o ${OWN} -g operator -m 664 /dev/null \
#	    ${DESTDIR}/etc/dumpdates
	${INSTALL} -d -o ${OWN} -g ${GRP} -m 655 ${DESTDIR}/var/run
	${INSTALL} -c -o ${OWN} -g wheel -m 644 /dev/null \
	    ${DESTDIR}/var/log/messages
#	${INSTALL} -c -o ${OWN} -g ${GRP} -m 644 /dev/null \
#	    ${DESTDIR}/usr/adm/daemonlog
#	${INSTALL} -c -o ${OWN} -g operator -m 664 /dev/null \
#	    ${DESTDIR}/usr/adm/shutdownlog
#	${INSTALL} -c -o ${OWN} -g ${GRP} -m 644 /dev/null \
#	    ${DESTDIR}/var/run/utmp
#	${INSTALL} -c -o nobody -g ${GRP} -m 644 /dev/null \
#	    ${DESTDIR}/var/db/find.codes
#	${INSTALL} -c -o ${OWN} -g wheel -m 644 /dev/null \
#	    ${DESTDIR}/usr/adm/lastlog
	${INSTALL} -c -o ${OWN} -g wheel -m 644 /dev/null \
	    ${DESTDIR}/var/log/wtmp
#	${INSTALL} -c -o uucp -g daemon -m 600 /dev/null \
#	    ${DESTDIR}/usr/adm/aculog

DIRS_DISTRIB=	bin sbin dev etc var/log var/run root usr usr/bin usr/sbin \
		usr/include usr/lib usr/libexec usr/share usr/games/lib
MDIRS_DISTRIB=	cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8

# Creates an empty DiscoBSD directory tree in DESTDIR.
distrib-dirs:
	if [ ! -d ${DESTDIR}/. ]; then \
		${INSTALL} -d -m 755 ${DESTDIR}; \
	fi
	for dir in ${DIRS_DISTRIB}; do \
		mkdir -p ${DESTDIR}/$$dir; \
	done
	for dir in ${MDIRS_DISTRIB}; do \
		mkdir -p ${DESTDIR}/usr/share/man/$$dir; \
	done
