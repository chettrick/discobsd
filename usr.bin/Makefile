#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ..; pwd
include $(TOPSRC)/target.mk

# XXX CFLAGS          += -Werror

YFLAGS=

MDSUBDIR!=if [ x"${MACHINE_ARCH}" = x"arm" ] ; then \
		echo "" ; \
	elif [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "adb adc-demo smlrc" ; \
	else \
		echo "" ; \
	fi

# Programs that live in subdirectories, and have makefiles of their own.
# /usr/bin
SUBDIR		= aout ar arch as awk basic calendar cc chflags \
                  chpass cpp dc diff emg env find fold forth \
                  fstat glcdtest id la lcc lcpp ld login make \
                  man med more nm passwd pdc picoc portio printf pwm \
                  ranlib re renice retroforth scm sed setty sl smallc \
                  smux uname wiznet xargs \
                  zmodem gtest msec compress tip tclsh \
                  uucp virus whereis yacc ${MDSUBDIR} \
                  basename cal cb cmp col comm \
                  diskspeed du fgrep file grep head hostid join \
                  last mesg nice od pagesize pr printenv \
                  rev size sort split sum \
                  tail tee time touch tr tsort tty uniq w wc who

# TODO: ccom lccom lex m4 pforth sre unixbench

# Shell scripts that need only be installed and are never removed.
#
SCRIPT		= false nohup true #lorder mkdep

# C programs that live in the current directory and need explicit make lines.
#
NSTD		= bc egrep

# Programs that must run setuid to root
#
SETUID		= mail su

# Programs that run set-group-id operator
#
OPERATOR	=

# Programs that must run set-group-id kmem.
#
KMEM		= vmstat

# Programs that must run set-group-id tty.
#
TTY		= wall write

# 'strip' is handled specially because 'install -s' now uses 'strip' and
# thus we can't do a 'install -s strip /bin/strip' without an error.
#
BINS		= $(NSTD) $(SETUID) $(OPERATOR) $(KMEM) $(TTY) $(SCRIPT) strip

all:		$(SUBDIR) $(BINS)

$(SUBDIR):	FRC
		$(MAKE) -C $@ $(MFLAGS)

FRC:

# $(SETUID) $(KMEM) $(OPERATOR) $(TTY) strip
%: %.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@.elf $< $(LIBS)
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && /bin/rm $@.elf

# Files listed in $(NSTD) have explicit make lines given below.

# bc egrep
%.c: %.y
		$(YACC) $(YFLAGS) $<
		-/bin/mv y.tab.c $@

$(SCRIPT):
		install -c $@.sh $@

install:	$(BINS)
		install $(NSTD) strip $(DESTDIR)/usr/bin/
		-for i in $(SUBDIR); do \
			$(MAKE) -C $$i $(MFLAGS) DESTDIR=$(DESTDIR) install; done
		-for i in $(SCRIPT); do install $$i.sh $(DESTDIR)/usr/bin/$$i; done
		-for i in $(SETUID); do \
			install -m 4751 $$i $(DESTDIR)/usr/bin/$$i; done
		-for i in $(OPERATOR); do \
			install -m 2751 $$i $(DESTDIR)/usr/bin/$$i; done
		-for i in $(KMEM); do \
			install -m 2751 $$i $(DESTDIR)/usr/bin/$$i; done
		-for i in $(TTY); do \
			install -m 2751 $$i $(DESTDIR)/usr/bin/$$i; done

clean:
		/bin/rm -f $(BINS) a.out core *.s *.o *.dis *.elf *~ y.tab.[ch] errs
		for i in $(SUBDIR); do (cd $$i; $(MAKE) $(MFLAGS) clean); done

depend:
		for i in $(BINS); do \
		    cc -M $(INCPATH) $$i.c | sed 's/\.o//' | \
		    awk ' ( if ($$1 != prev) \
			( if (rec != "") print rec; rec = $$0; prev = $$1; ) \
			else ( if (length(rec $$2) > 78) ( print rec; rec = $$0; ) \
			else rec = rec " " $$2 ) ) \
			END ( print rec ) ' >> makedep; done
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
