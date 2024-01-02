TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

SRCS		= diff.c diffdir.c diffreg.c
OBJS		= diff.o diffdir.o diffreg.o

DIFF            = /usr/bin/diff
DIFFH           = /usr/libexec/diffh
PR              = /usr/bin/pr
CFLAGS          += -DDIFF='"${DIFF}"' -DDIFFH='"${DIFFH}"' -DPR='"${PR}"'

all:            diff diffh

diff:           ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o diff.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S diff.elf > diff.dis
		${SIZE} diff.elf
		${ELF2AOUT} diff.elf $@ && rm diff.elf

diffh:          diffh.o
		${CC} ${CFLAGS} ${LDFLAGS} -o diffh.elf diffh.o ${LIBS}
		${OBJDUMP} -S diffh.elf > diffh.dis
		${SIZE} diffh.elf
		${ELF2AOUT} diffh.elf $@ && rm diffh.elf

clean:
		rm -f *.o *.elf ${MAN} diff diffh *.elf *.dis tags *~

install:        all
		install diff ${DESTDIR}/usr/bin/diff
		install diffh ${DESTDIR}/usr/libexec/diffh
