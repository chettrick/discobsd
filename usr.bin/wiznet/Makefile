TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS          += -O -Wall
LIBS            += -lwiznet -lc

EXAMPLES        = telnet \
                  ntpdate \
                  chat-server \
                  web-client \
                  web-server

all:            $(EXAMPLES)

web-client:     web-client.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf web-client.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

web-server:     web-server.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf web-server.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

telnet:         telnet.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf telnet.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

ntpdate:        ntpdate.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ntpdate.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

chat-server:    chat-server.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf chat-server.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

clean:
		rm -f *.o *.elf *.dis $(EXAMPLES)

install:        all
		install ${EXAMPLES} ${DESTDIR}/usr/bin/
