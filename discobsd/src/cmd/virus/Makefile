TOPSRC          = $(shell cd ../../..; pwd)
include $(TOPSRC)/target.mk

# XXX CFLAGS          += -Wall -Werror -Os
CFLAGS          += -Wall -Os

virus:          virus.c
		$(CC) $(LDFLAGS) -o virus.elf virus.c $(LIBS) -ltermcap
		$(SIZE) virus.elf
		$(ELF2AOUT) virus.elf $@

install:        virus
		install virus ${DESTDIR}/bin/vi

clean:
		rm -f *.o virus.elf virus
