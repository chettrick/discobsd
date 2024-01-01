TOPSRC!=cd ../..; pwd
include $(TOPSRC)/target.mk

OBJS            = message.o access.o command.o expr.o findfn.o format.o input.o \
                  main.o opset.o output.o pcs.o print.o \
                  runpcs.o setup.o sym.o

CFLAGS		+= -DNUM_SYMS_CACHE=50 -Wall

all:            adb

adb:            $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o adb.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S adb.elf > adb.dis
		${SIZE} adb.elf
		${ELF2AOUT} adb.elf $@ && rm adb.elf

clean:
		rm -f *.o *.0 *.elf adb *.elf *.dis tags *~

install:        all
		install adb ${DESTDIR}/usr/bin/

$(OBJS): defs.h
