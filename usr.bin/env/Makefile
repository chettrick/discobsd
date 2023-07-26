TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk
#include $(TOPSRC)/cross.mk

OBJS            = env.o

CFLAGS		+= -Wall -DGCC_COMPAT
CFLAGS          += -DVERSSTR=\"1.0\" \
                   -DSTDINC=\"/usr/include\" \
                   -DLIBDIR=\"/usr/lib\" \
                   -DLIBEXECDIR=\"/usr/libexec\"
                   -DCRT0FILE=\"/usr/lib/crt0.o\" \
                   -DCRT0FILE_PROFILE=\"/usr/lib/gcrt0.o\"

all:            env

env:            $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o env.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S env.elf > env.dis
		${SIZE} env.elf
		${ELF2AOUT} env.elf $@ && rm env.elf

clean:
		rm -f *.o *.0 *.elf env *.elf *.dis tags *~

install:        all
		install env ${DESTDIR}/usr/bin/
