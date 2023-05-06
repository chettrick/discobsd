# Copyright (c) 1986 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
# This makefile is designed to be run as:
#	make
#
# The `make' will compile everything, including a kernel, utilities
# and a root filesystem image.

# Override the default port with:
# $ gmake MACHINE=pic32 MACHINE_ARCH=mips
#
MACHINE=	stm32
MACHINE_ARCH=	arm

# Filesystem and swap sizes.
FS_MBYTES       = 100
U_MBYTES        = 100
SWAP_MBYTES     = 2

# SD card filesystem image for $(MACHINE).
FSIMG		= distrib/$(MACHINE)/sdcard.img

# Set this to the device name for your SD card.  With this
# enabled you can use "make installfs" to copy the sdcard.img
# to the SD card.

#SDCARD          = /dev/sdb

#
# C library options: passed to libc makefile.
# See lib/libc/Makefile for explanation.
#
DEFS		=

FSUTIL		= tools/fsutil/fsutil

-include Makefile.user

TOPSRC=		$(shell pwd)
DESTDIR?=	${TOPSRC}/distrib/obj/destdir.${MACHINE}
KCONFIG=	${TOPSRC}/tools/kconfig/kconfig

all:		symlinks
		$(MAKE) -C tools
		$(MAKE) kernel
		$(MAKE) -C etc DESTDIR=${DESTDIR} distrib-dirs
		$(MAKE) -C include includes
		$(MAKE) -C share
		$(MAKE) -C lib
		$(MAKE) -C bin
		$(MAKE) -C sbin
		$(MAKE) -C libexec
		$(MAKE) -C usr.bin
		$(MAKE) -C usr.sbin
		$(MAKE) -C games
		$(MAKE) -C share DESTDIR=${DESTDIR} install
		$(MAKE) -C lib DESTDIR=${DESTDIR} install
		$(MAKE) -C bin DESTDIR=${DESTDIR} install
		$(MAKE) -C sbin DESTDIR=${DESTDIR} install
		$(MAKE) -C libexec DESTDIR=${DESTDIR} install
		$(MAKE) -C usr.bin DESTDIR=${DESTDIR} install
		$(MAKE) -C usr.sbin DESTDIR=${DESTDIR} install
		$(MAKE) -C games DESTDIR=${DESTDIR} install
		sudo $(MAKE) -C etc DESTDIR=${DESTDIR} MACHINE=${MACHINE} distribution
		$(MAKE) fs

kernel:         $(KCONFIG)
		$(MAKE) -C sys/$(MACHINE) all

fs:		$(FSIMG)

.PHONY:		$(FSIMG)
$(FSIMG):	$(FSUTIL) distrib/$(MACHINE)/md.$(MACHINE) distrib/base/mi.home
		rm -f $@ distrib/$(MACHINE)/_manifest
		cat distrib/base/mi distrib/$(MACHINE)/md.$(MACHINE) > distrib/$(MACHINE)/_manifest
		$(FSUTIL) --repartition=fs=$(FS_MBYTES)M:swap=$(SWAP_MBYTES)M:fs=$(U_MBYTES)M $@
		sudo $(FSUTIL) --new --partition=1 --manifest=distrib/$(MACHINE)/_manifest $@ ${DESTDIR}
# In case you need a separate /home partition,
# uncomment the following line.
		$(FSUTIL) --new --partition=3 --manifest=distrib/base/mi.home $@ home

$(FSUTIL):
		cd tools/fsutil; $(MAKE)

$(KCONFIG):
		$(MAKE) -C tools/kconfig

clean:
		rm -f *~
		rm -f include/machine
		for dir in tools share lib bin sbin libexec usr.bin usr.sbin games; do \
			$(MAKE) -C $$dir -k clean; done

cleanfs:
		rm -f distrib/$(MACHINE)/_manifest
		rm -f $(FSIMG)

cleanall:       clean
		$(MAKE) -C sys/$(MACHINE) -k clean
		rm -f sys/$(MACHINE)/*/unix.hex

symlinks:
		rm -f include/machine
		ln -s $(MACHINE) include/machine

installfs:
ifdef SDCARD
		@[ -f $(FSIMG) ] || $(MAKE) $(FSIMG)
		sudo dd bs=32k if=$(FSIMG) of=$(SDCARD)
else
		@echo "Error: No SDCARD defined."
endif

# TODO: make it relative to Target
installflash:
		sudo pic32prog sys/pic32/fubarino/unix.hex

# TODO: make it relative to Target
installboot:
		sudo pic32prog sys/pic32/fubarino/bootloader.hex

# STM32-specific emulator and debugger.
-include Makefile.inc
