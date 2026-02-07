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

TOPSRC!=cd ..; pwd
include ${TOPSRC}/share/mk/sys.mk

DESTDIR=	/foobar
RELEASEDIR=	/foobaz
OWN=		root
GRP=		wheel

TZDIR=		/usr/share/zoneinfo
LOCALTIME=	Canada/Mountain

FILES=		fstab gettytab group motd passwd rc rc.local shells ttys

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
MDIRS_DISTRIB=	cat1 cat2 cat3 cat4 cat5 cat6 cat7 cat8 cat9

# XXX Keep in sync with SUBDIR in sys/arch/${MACHINE}/compile/Makefile
DIRS_RELEASE!=if [ x"${MACHINE}" = x"stm32" ] ; then \
		echo "F405WEACTCORE F411RENUCLEO F412GDISCO F412WEACTCORE \
			F413HDISCO F469IDISCO F4DISCOVERY" ; \
	elif [ x"${MACHINE}" = x"pic32" ] ; then \
		echo "BAREMETAL DUINOMITE DUINOMITE_EMEGA EXPLORER16 \
			FUBARINO MAX32 MAXIMITE MAXIMITE_COLOR MMB_MX7 \
			PICADILLO PINGUINO_MICRO QUICK240 \
			SDXL SNADPIC STARTER_KIT UBW32 WF32" ; \
	else \
		echo "FAIL" ; \
	fi

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

# Creates and populates a DiscoBSD release directory tree in RELEASEDIR.
release-dirs:
	if [ ! -d ${RELEASEDIR}/. ]; then \
		${INSTALL} -d -m 755 ${RELEASEDIR}; \
	fi
	${INSTALL} -d -m 755 ${RELEASEDIR}/${MACHINE}
	cd ${RELEASEDIR}/${MACHINE} && \
	for dir in ${DIRS_RELEASE}; do \
		mkdir -p $$dir; \
		cp ${TOPSRC}/sys/arch/${MACHINE}/compile/$$dir/unix* $$dir; \
	done
	cp -f ${TOPSRC}/distrib/${MACHINE}/sdcard.img ${RELEASEDIR}/${MACHINE}
	cp -f ${TOPSRC}/distrib/${MACHINE}/README.md ${RELEASEDIR}/${MACHINE}
	cp -f ${TOPSRC}/README.md ${RELEASEDIR}

release: release-dirs
	cd ${RELEASEDIR} && \
	tar czvf DiscoBSD_${OSRev}_${MACHINE}.tar.gz ${MACHINE} && \
	zip -r DiscoBSD_${OSRev}_${MACHINE}.zip ${MACHINE}

.PHONY:	all distribution distribution-etc-root-var \
	distrib-dirs release-dirs release clean
