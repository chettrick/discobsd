TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

OBJS            = cc.o

CFLAGS          += -Wall
CFLAGS          += -DVERSSTR=\"1.0\" \
                   -DSTDINC=\"/usr/include\" \
                   -DLIBDIR=\"/usr/lib\" \
                   -DLIBEXECDIR=\"/usr/libexec\" \
                   -DCRT0FILE=\"/usr/lib/crt0.o\" \
                   -DCRT0FILE_PROFILE=\"/usr/lib/mcrt0.o\"

all:            cc

cc:             $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o cc.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S cc.elf > cc.dis
		${SIZE} cc.elf
		${ELF2AOUT} cc.elf $@ && rm cc.elf

clean:
		rm -f *.o *.0 *.elf cc *.elf *.dis tags *~

install:        all
		${INSTALL} cc ${DESTDIR}/usr/bin/
		${INSTALL} cc ${DESTDIR}/usr/bin/scc
		${INSTALL} cc ${DESTDIR}/usr/bin/lcc
