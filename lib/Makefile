#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ..; pwd
include ${TOPSRC}/share/mk/sys.mk

SUBDIR=	libc_aout \
	startup-${MACHINE_ARCH} libc libm libcurses libtermlib \
	libutil libvmf libtcl libreadline libgpanel libwiznet

all:		$(SUBDIR)

$(SUBDIR):	FRC
		$(MAKE) -C $@

FRC:

install:        elf32-$(MACHINE_ARCH).ld
		-for i in $(SUBDIR); do \
			${MAKE} -C $$i DESTDIR=${DESTDIR} install; done
#		cp -p elf32-$(MACHINE_ARCH).ld $(DESTDIR)/usr/lib/elf32-$(MACHINE_ARCH).ld

clean:
		rm -f a.out core *.s *.o *.a *~
		-for i in $(SUBDIR); do $(MAKE) -C $$i clean; done
