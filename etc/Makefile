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

FILES=	fstab gettytab group etc.${MACHINE}/motd passwd rc rc.local shells ttys

all install depend lint tags:

clean:
	rm -f *~

distribution: distribution-etc-root-var

distribution-etc-root-var: distrib-dirs
	-@mkdir -p ${DESTDIR}/dev
	@chmod 755 ${DESTDIR}/dev
	-@mkdir -p ${DESTDIR}/etc
	@chmod 755 ${DESTDIR}/etc
	@chown ${OWN} ${DESTDIR}/dev ${DESTDIR}/etc
	@chgrp ${GRP} ${DESTDIR}/dev ${DESTDIR}/etc

	install -c -o ${OWN} -g ${GRP} -m 644 ${FILES} ${DESTDIR}/etc
	install -c -o root -g wheel -m 600 shadow ${DESTDIR}/etc
#	install -c -o root -g wheel -m 600 master.passwd ${DESTDIR}/etc
#	install -c -o ${OWN} -g ${GRP} -m 555 MAKEDEV.local MAKEDEV \
#	    ${DESTDIR}/dev
	install -c -o ${OWN} -g ${GRP} -m 555 MAKEDEV \
	    ${DESTDIR}/etc
	cd root; \
		install -c -o root -g wheel -m 644 dot.profile \
		    ${DESTDIR}/root/.profile; \
		install -c -o root -g wheel -m 644 dot.profile \
		    ${DESTDIR}/.profile
#	install -c -o ${OWN} -g operator -m 664 /dev/null \
#	    ${DESTDIR}/etc/dumpdates
	-@mkdir -p ${DESTDIR}/var/run
	@chmod 655 ${DESTDIR}/var/run
	@chown ${OWN} ${DESTDIR}/var/run
	@chgrp ${GRP} ${DESTDIR}/var/run
	install -c -o ${OWN} -g wheel -m 644 /dev/null \
	    ${DESTDIR}/var/log/messages
#	install -c -o ${OWN} -g ${GRP} -m 644 /dev/null \
#	    ${DESTDIR}/usr/adm/daemonlog
#	install -c -o ${OWN} -g operator -m 664 /dev/null \
#	    ${DESTDIR}/usr/adm/shutdownlog
#	install -c -o ${OWN} -g ${GRP} -m 644 /dev/null \
#	    ${DESTDIR}/var/run/utmp
#	install -c -o nobody -g ${GRP} -m 644 /dev/null \
#	    ${DESTDIR}/var/db/find.codes
#	install -c -o ${OWN} -g wheel -m 644 /dev/null \
#	    ${DESTDIR}/usr/adm/lastlog
	install -c -o ${OWN} -g wheel -m 644 /dev/null \
	    ${DESTDIR}/var/log/wtmp
#	install -c -o uucp -g daemon -m 600 /dev/null \
#	    ${DESTDIR}/usr/adm/aculog

DIRS_DISTRIB=	bin sbin etc share games/lib var/log root \
		usr/include usr/lib usr/libexec
MDIRS_DISTRIB=	cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8

# Creates an empty DiscoBSD directory tree in DESTDIR.
distrib-dirs:
	if [ ! -d ${DESTDIR}/. ]; then \
		install -d -m 755 ${DESTDIR}; \
	fi
	for dir in ${DIRS_DISTRIB}; do \
		mkdir -p ${DESTDIR}/$$dir; \
	done
	for dir in ${MDIRS_DISTRIB}; do \
		mkdir -p ${DESTDIR}/share/man/$$dir; \
	done
