TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS          += -Wall -O0

virus:          virus.c
		${CC} ${CFLAGS} ${LDFLAGS} -o virus.elf virus.c ${LIBS} -ltermcap
		$(SIZE) virus.elf
		$(ELF2AOUT) virus.elf $@

install:        virus
		install virus ${DESTDIR}/usr/bin/vi

clean:
		rm -f *.o virus.elf virus
