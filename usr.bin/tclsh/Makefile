TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

PROG=		tclsh
OBJS=		tclsh.o

# XXX -Wno-pointer-sign until unsigned char * issues in libtcl are remedied.
CFLAGS+=	-Os -pedantic -Wall -Wno-pointer-sign
LDADD=		-ltcl

all:		${PROG}

${PROG}:	${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LDADD} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@ && rm $@.elf

clean:
		rm -f *.o *.0 *.elf ${PROG} *.dis a.out tags *~

install:	all
		install ${PROG} ${DESTDIR}/usr/bin/
