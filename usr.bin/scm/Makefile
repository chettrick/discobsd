TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Os -Wall

OBJS            = scm.o func.o
TESTS           = tests/*.scm

all:            scm ${MAN}

scm:            ${OBJS}
		${CC} ${CFLAGS} ${LDFLAGS} -o scm.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S scm.elf > scm.dis
		${SIZE} scm.elf
		${ELF2AOUT} scm.elf $@ && rm scm.elf

clean:
		rm -f *.o scm *.elf *.dis *~ tests/*.out

install:        all
		install scm ${DESTDIR}/usr/bin/

tests:          scm $(TESTS) ALWAYS
		for i in $(TESTS); do \
                        base=`basename $$i .scm`;\
                        ./scm -v $$i > tests/$$base.out;\
                        diff tests/$$base.expected tests/$$base.out; done

ALWAYS:

expected:       scm $(TESTS)
		for i in $(TESTS); do \
                        base=`basename $$i .scm`;\
                        ./scm -v $$i > tests/$$base.expected; done
