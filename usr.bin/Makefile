#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ..; pwd
include $(TOPSRC)/target.mk

MDSUBDIR!=if [ x"${MACHINE_ARCH}" = x"arm" ] ; then \
		echo "" ; \
	elif [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "adb adc-demo smlrc" ; \
	else \
		echo "" ; \
	fi

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
                  tail tee time touch tr tsort tty uniq w wc who \
                  bc egrep mail su vmstat wall write

# TODO: ccom lccom lex m4 pforth sre unixbench

# Shell scripts that need only be installed and are never removed.
#
SCRIPT		= false nohup true #lorder mkdep

# 'strip' is handled specially because 'install -s' now uses 'strip' and
# thus we can't do a 'install -s strip /bin/strip' without an error.
#
BINS		= $(SCRIPT) strip

all:		$(SUBDIR) $(BINS)

$(SUBDIR):	FRC
		$(MAKE) -C $@ $(MFLAGS)

FRC:

# strip
%: %.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@.elf $< $(LIBS)
		$(OBJDUMP) -S $@.elf > $@.dis
		$(SIZE) $@.elf
		$(ELF2AOUT) $@.elf $@ && /bin/rm $@.elf

$(SCRIPT):
		install -c $@.sh $@

install:	$(BINS)
		install strip $(DESTDIR)/usr/bin/
		-for i in $(SUBDIR); do \
			$(MAKE) -C $$i $(MFLAGS) DESTDIR=$(DESTDIR) install; done
		-for i in $(SCRIPT); do install $$i.sh $(DESTDIR)/usr/bin/$$i; done

clean:
		/bin/rm -f $(BINS) a.out core *.s *.o *.dis *.elf *~ y.tab.[ch] errs
		for i in $(SUBDIR); do (cd $$i; $(MAKE) $(MFLAGS) clean); done
