#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC		= $(shell cd ..; pwd)
include $(TOPSRC)/target.mk

# Programs that live in subdirectories, and have makefiles of their own.
#
SUBDIR		= startup-$(MACHINE_ARCH) libc libm libutil libtermlib \
		  libcurses libvmf libtcl libwiznet libreadline libgpanel \
		  libc_aout cmd games man

all:		$(SUBDIR)

$(SUBDIR):	FRC
		$(MAKE) -C $@

FRC:

install:        elf32-$(MACHINE_ARCH).ld
		-for i in $(SUBDIR); do \
			$(MAKE) -C $$i DESTDIR=$(DESTDIR) all install; done
#		cp -p elf32-$(MACHINE_ARCH).ld $(DESTDIR)/lib/elf32-$(MACHINE_ARCH).ld

clean:
		rm -f a.out core *.s *.o *.a *~
		-for i in $(SUBDIR); do $(MAKE) -C $$i clean; done
