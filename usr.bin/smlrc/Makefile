TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS          = -Os -Wall -DMIPS -DNO_ANNOTATIONS -DNO_PREPROCESSOR \
                  -DNO_PPACK -D_RETROBSD -D__SMALLER_C_SCHAR__ \
                  -D__SMALLER_C__ -D__SMALLER_C_32__ -DSTATIC \
                  -DNO_EXTRA_WARNS -DSYNTAX_STACK_MAX=3200

LIBS		= -lc

ARCH            = mips

OBJS            = smlrc.o lb.o

all:		smlrc

smlrc:          $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o $@.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S $@.elf > $@.dis
		${SIZE} $@.elf
		${ELF2AOUT} $@.elf $@

install:        smlrc
		${INSTALL} smlrc ${DESTDIR}/usr/libexec/


clean:
		rm -f *.o smlrc smlrc.dis smlrc.elf
###
smlrc.o: smlrc.c fp.c cgmips.c
		${CC} ${CFLAGS} -mips16 smlrc.c -c -o $@
