#
# Public Domain.  1995/03/13 - Steven Schultz
#
TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -I${TOPSRC}/sys/include

SRCS            = globread.c globwrite.c
OBJS            = globread.o globwrite.o

all:            globread globwrite globdump

globread:	globread.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf globread.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

globdump:	globdump.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf globdump.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

globwrite:	globwrite.o
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf globwrite.o ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@


clean:
		rm -f *.o *.elf ${MAN} *.elf *.dis tags *~  globread globwrite globdump

depend:         ${SRCS}
		mkdep ${CFLAGS} ${SRCS}

install:        all
		install -m 755 globread ${DESTDIR}/usr/bin
		install -m 755 globdump ${DESTDIR}/usr/bin
		install -m 755 globwrite ${DESTDIR}/usr/bin

lint:           ${SRCS}
		lint -hax ${SRCS}

tags:           ${SRCS}
		ctags ${SRCS}
