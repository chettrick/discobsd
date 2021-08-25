#
# Copyright (c) 1987 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
#	crt0	Normal C run time startoff
#
TOPSRC          = $(shell cd ../..; pwd)
include $(TOPSRC)/target.mk
vpath %.c $(TOPSRC)/src/startup-$(MACHINE_ARCH)

ifeq ($(MACHINE_ARCH), mips)
CFLAGS          = -Os -B$(TOPSRC)/lib/ -Wa,-x $(DEFS)
else
CFLAGS          = -Os $(DEFS) -Wall -Werror
endif

OBJS		= ../crt0.o

all:		$(OBJS)

../crt0.o:	crt0.c
		$(CC) $(DEFS) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJS) *~
