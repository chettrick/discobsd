TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

SRCS            = find.c bigram.c code.c

all:            find bigram code

find:           find.o
		${CC} ${CFLAGS} ${LDFLAGS} -o find.elf find.o ${LIBS}
		${OBJDUMP} -S find.elf > find.dis
		${SIZE} find.elf
		${ELF2AOUT} find.elf $@ && rm find.elf

bigram:         bigram.o
		${CC} ${CFLAGS} ${LDFLAGS} -o bigram.elf bigram.o ${LIBS}
		${OBJDUMP} -S bigram.elf > bigram.dis
		${SIZE} bigram.elf
		${ELF2AOUT} bigram.elf $@ && rm bigram.elf

code:           code.o
		${CC} ${CFLAGS} ${LDFLAGS} -o code.elf code.o ${LIBS}
		${OBJDUMP} -S code.elf > code.dis
		${SIZE} code.elf
		${ELF2AOUT} code.elf $@ && rm code.elf

clean:
		rm -f *.o *.elf find bigram code *.elf *.dis tags *~

install:        all
		${INSTALL} find ${DESTDIR}/usr/bin/find
		${INSTALL} bigram ${DESTDIR}/usr/libexec/bigram
		${INSTALL} code ${DESTDIR}/usr/libexec/code
		${INSTALL} updatedb.sh ${DESTDIR}/usr/libexec/updatedb

depend:
		chmod u+w Makefile
		mkdep ${CFLAGS} ${SRCS}
