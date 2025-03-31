#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ..; pwd
include ${TOPSRC}/share/mk/sys.mk

MDSUBDIR!=if [ x"${MACHINE_ARCH}" = x"arm" ] ; then \
		echo "lex" ; \
	elif [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "adb smlrc" ; \
	else \
		echo "" ; \
	fi

SUBDIR		= aout ar arch as awk basic calendar cc chflags \
                  chpass cpp dc diff emg env find fold forth \
                  fstat glcdtest id la lcc lcpp ld login m4 make \
                  man med more nm passwd pdc picoc portio printf pwm \
                  ranlib re renice retroforth scm sed setty sl smallc \
                  smux uname wiznet xargs xinstall \
                  zmodem gtest msec compress tip tclsh \
                  uucp virus whereis yacc ${MDSUBDIR} \
                  basename cal cb cmp col comm \
                  diskspeed du fgrep file grep head hostid join \
                  last mesg nice od pagesize pr printenv \
                  rev size sort split sum \
                  tail tee time touch tr tsort tty uniq w wc who \
                  bc egrep mail su vmstat wall write \
                  strip false nohup true

# TODO: ccom lccom pforth sre unixbench adc-demo
#       lorder mkdep

all:		$(SUBDIR)

$(SUBDIR):	FRC
		$(MAKE) -C $@ $(MFLAGS)

FRC:

install:
		-for i in $(SUBDIR); do \
			$(MAKE) -C $$i $(MFLAGS) DESTDIR=$(DESTDIR) install; done

clean:
		/bin/rm -f a.out core *.s *.o *.dis *.elf *~ y.tab.[ch] errs
		for i in $(SUBDIR); do (cd $$i; $(MAKE) $(MFLAGS) clean); done
