#
#	@(#)Makefile	5.5 (Berkeley) 9/18/85
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

OBJS 		= compress.o

#COMFLAGS       = -DBSD4_2 -O -DSACREDMEM=256000

CFLAGS          += -DUSERMEM=1024 -Wall

compress: $(OBJS)
	${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
	${OBJDUMP} -S $@.elf > $@.dis
	${SIZE} $@.elf
	${ELF2AOUT} $@.elf $@

# USERMEM may have to be set by hand.  It should contain the amount of
# available user memory in bytes.  Set it to zero, for physical memory
# less than 1 Meg.
USERMEM:
	sh usermem.sh > USERMEM

install: compress
	${INSTALL} compress ${DESTDIR}/usr/bin/compress
	${INSTALL} compress ${DESTDIR}/usr/bin/uncompress
	${INSTALL} compress ${DESTDIR}/usr/bin/zcat

# Temporarily don't delete USERMEM.  When chroot'ed to /nbsd, usermem.sh
# fails totally.
clean:
	rm -f compress core errs compress.o *.elf *.dis
#	rm -f compress USERMEM core errs
