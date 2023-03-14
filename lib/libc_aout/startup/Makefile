#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	crt0	Normal C run time startoff
#
TOPSRC          = $(shell cd ../../..; pwd)
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/lib/startup-$(MACHINE_ARCH)

CFLAGS          = -Os $(DEFS) -Wall -Werror

ifeq ($(MACHINE_ARCH), mips)
CFLAGS          += -Wa,-x -B$(TOPSRC)/lib/libc_aout/
endif

OBJS		= ../crt0.o

all:		$(OBJS)

../crt0.o:	crt0.c
		$(CC) $(DEFS) $(CFLAGS) -c $< -o $@

install:	../crt0.o
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../crt0.o ${DESTDIR}/usr/lib/crt0.o

clean:
		rm -f $(OBJS) *~
