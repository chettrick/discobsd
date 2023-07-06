TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk
#include $(TOPSRC)/cross.mk

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
		${CC} ${LDFLAGS} -o cc.elf $(OBJS) ${LIBS}
		${OBJDUMP} -S cc.elf > cc.dis
		${SIZE} cc.elf
		${ELF2AOUT} cc.elf $@ && rm cc.elf

clean:
		rm -f *.o *.0 *.elf cc *.elf *.dis tags *~

install:        all
		install cc ${DESTDIR}/usr/bin/
		install cc ${DESTDIR}/usr/bin/scc
		install cc ${DESTDIR}/usr/bin/lcc
