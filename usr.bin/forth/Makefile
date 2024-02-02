TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS          += -Wall
LIBS            += -lm -lc

OBJS            = forth.o func.o

all:            forth

forth:          ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o forth.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S forth.elf > forth.dis
		${SIZE} forth.elf
		${ELF2AOUT} forth.elf $@ && rm forth.elf

clean:
		rm -f *.o *.elf forth *.dis *~

func.txt:	func.c
		grep '^ \*+' func.c | expand | sed 's/^ \*+ *$$//' |\
		sed 's/^ \*+ //' > func.txt

install:        all
		${INSTALL} forth ${DESTDIR}/usr/bin/

###
forth.o: forth.c io.h forth.h
func.o: func.c io.h forth.h
