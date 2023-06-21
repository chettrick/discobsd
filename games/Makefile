#
# Copyright (c) 1980 Regents of the University of California.
# All rights reserved.  The Berkeley software License Agreement
# specifies the terms and conditions for redistribution.
#
TOPSRC!=cd ..; pwd
include $(TOPSRC)/target.mk
#include $(TOPSRC)/cross.mk

SUBDIR          = adventure atc backgammon battlestar boggle btlgammon \
                  caesar cribbage fortune hangman mille monop pom pig quiz \
                  robots rogue sail snake trek \
                  banner aclock arithmetic bcd factor fish morse \
                  number ppt wump primes rain worm worms canfield

all:            $(SUBDIR)

$(SUBDIR):      FRC
		$(MAKE) -C $@ $(MFLAGS)

FRC:

install:
		-for i in $(SUBDIR); do \
			${MAKE} -C $$i $(MFLAGS) DESTDIR=$(DESTDIR) install; done

clean:
		rm -f a.out core *.s *.o *.dis *.elf
		-for i in $(SUBDIR); do ${MAKE} -C $$i $(MFLAGS) clean; done
