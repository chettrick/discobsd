# Copyright (c) 1986 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
# This makefile is designed to be run as:
#	make
#
# The `make' will compile everything, including a kernel, utilities
# and a root filesystem image.

MACHINE		?= stm32
MACHINE_ARCH	?= arm

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

TOPSRC       = $(shell pwd)
KCONFIG      = $(TOPSRC)/tools/kconfig/kconfig

all:		symlinks
		#$(MAKE) -C tools
		$(MAKE) -C lib
		$(MAKE) -C src install
		$(MAKE) kernel
		$(MAKE) fs

kernel:         $(KCONFIG)
		$(MAKE) -C sys/$(MACHINE) all

fs:		$(FSIMG)

.PHONY:		$(FSIMG)
$(FSIMG):	$(FSUTIL) distrib/$(MACHINE)/md.$(MACHINE) distrib/base/mi.home
		rm -f $@ distrib/$(MACHINE)/_manifest
		cat distrib/base/mi distrib/$(MACHINE)/md.$(MACHINE) > distrib/$(MACHINE)/_manifest
		$(FSUTIL) --repartition=fs=$(FS_MBYTES)M:swap=$(SWAP_MBYTES)M:fs=$(U_MBYTES)M $@
		$(FSUTIL) --new --partition=1 --manifest=distrib/$(MACHINE)/_manifest $@ .
# In case you need a separate /home partition,
# uncomment the following line.
		$(FSUTIL) --new --partition=3 --manifest=distrib/base/mi.home $@ home

$(FSUTIL):
		cd tools/fsutil; $(MAKE)

$(KCONFIG):
		$(MAKE) -C tools/kconfig

clean:
		rm -f *~
		for dir in tools lib src sys/$(MACHINE); do $(MAKE) -C $$dir -k clean; done

cleanfs:
		rm -f distrib/$(MACHINE)/_manifest
		rm -f $(FSIMG)

cleanall:       clean
		$(MAKE) -C lib clean
		rm -f sys/$(MACHINE)/*/unix.hex bin/* sbin/* libexec/*
		rm -f games/[a-k]* games/[m-z]* share/man/cat*/*
		rm -f games/lib/adventure.dat games/lib/cfscores
		rm -f share/re.help share/emg.keys
		rm -f share/misc/more.help share/misc/yaccpar
		rm -f etc/termcap etc/remote etc/phones etc/motd
		rm -f include/machine
		rm -f var/log/aculog
		rm -rf var/lock share/unixbench

symlinks:
		rm -f include/machine
		ln -s $(MACHINE) include/machine
		if [ -f etc/etc.$(MACHINE)/motd ]; then \
			cp -p etc/etc.$(MACHINE)/motd etc/motd; \
		fi

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
