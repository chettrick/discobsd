# emg Makefile
# for RetroBSD

TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

# Some basic CFLAGS.
CFLAGS +=	-Os -Wall

# With the extra LDFLAGS, save some bytes.
CFLAGS +=	-ffunction-sections -fdata-sections

# with CFLAGS+=	-ffunction-sections -fdata-sections
LDFLAGS +=	-Wl,--gc-sections

LIBS =		-ltermcap -lc

MAN =		emg.0
MANSRC =	emg.1

OBJS =	basic.o buffer.o display.o file.o fileio.o line.o main.o \
	random.o region.o search.o tcap.o ttyio.o word.o

all:	emg ${MAN}

emg:	${OBJS}
	${CC} ${CFLAGS} ${LDFLAGS} -o emg.elf ${OBJS} ${LIBS}
	${OBJDUMP} -S emg.elf > emg.dis
	${SIZE} emg.elf
	${ELF2AOUT} emg.elf $@ && rm emg.elf

${MAN}:	${MANSRC}
	${MANROFF} ${MANSRC} > ${MAN}

install: all
	${INSTALL} emg ${DESTDIR}/usr/bin/emg
	cp ${MAN} ${DESTDIR}/usr/share/man/cat1/
	cp -p emg.keys ${DESTDIR}/usr/share/misc/emg.keys

clean:
	rm -f *.o *~ *.core *.bak *.dis emg ${MAN}
