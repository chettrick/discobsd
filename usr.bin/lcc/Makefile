TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

OBJS            = lcc.o retrobsd.o
MAN             = lcc.0
MANSRC          = lcc.1

CFLAGS		+= -Wall -Os

all:            lcc $(MAN)

lcc:            $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o lcc.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S lcc.elf > lcc.dis
		${SIZE} lcc.elf
		${ELF2AOUT} lcc.elf $@ && rm lcc.elf

bprint:         bprint.o ../lccom/profio.c
		${CC} ${CFLAGS} ${LDFLAGS} -o bprint.elf bprint.o ${LIBS}
		${OBJDUMP} -S bprint.elf > bprint.dis
		${SIZE} bprint.elf
		${ELF2AOUT} bprint.elf $@ && rm bprint.elf

ops:            ops.o ../lccom/c.h ../lccom/ops.h
		${CC} ${CFLAGS} ${LDFLAGS} -o ops.elf ops.o ${LIBS}
		${OBJDUMP} -S ops.elf > ops.dis
		${SIZE} ops.elf
		${ELF2AOUT} ops.elf $@ && rm ops.elf

$(MAN):         ${MANSRC}
		${MANROFF} ${MANSRC} > $@

clean:
		rm -f *.o *.0 *.elf lcc bprint ops *.dis *~

install:        all
		install lcc ${DESTDIR}/usr/bin/
		cp lcc.0 ${DESTDIR}/usr/share/man/cat1/
