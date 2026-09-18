TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

CFLAGS		+= -Os -ansi

YACC            = bison -y
LIBS            = -lm -lc
FILES           = awk.lx.o b.o main.o tran.o lib.o run.o parse.o proctab.o freeze.o
SOURCE          = awk.def.h awk.g.y awk.lx.l b.c lib.c main.c parse.c \
                  proc.c freeze.c run.c tran.c

awk:            $(FILES) awk.g.o
		${CC} ${CFLAGS} ${LDFLAGS} -o awk.elf ${FILES} awk.g.o ${LIBS}
		${OBJDUMP} -S awk.elf > awk.dis
		${SIZE} awk.elf
		${ELF2AOUT} awk.elf $@ && rm awk.elf

# awk.g.y is the yacc grammar. The parser awk.g.c is generated from it by
# make's built-in .y.c rule, which runs yacc WITHOUT -d and so never emits
# the token header y.tab.h that awk.lx.l #includes as awk.h. On a clean
# build ("make clean" removes awk.h and y.tab.*), awk.lx.o then failed with
# no awk.h to include. Generate the header straight from the grammar here.
# The parser and this header come from the same yacc on the same awk.g.y,
# so their token numbers agree.
awk.h:          awk.g.y
		${YACC} -d awk.g.y
		-cmp -s y.tab.h awk.h || cp y.tab.h awk.h
		rm -f y.tab.c y.tab.h

$(FILES):	awk.h awk.def.h

token.h:	awk.h tokenscript
		ex - < tokenscript

install:        awk
		${INSTALL} awk ${DESTDIR}/usr/bin/

#profile:	awk.g.o $(FILES) mon.o
#		${HOST_CC} -p -i awk.g.o ${FILES} mon.o -lm

lint:
		lint -spu b.c main.c tran.c run.c lib.c parse.c -lm |\
			egrep -v '^(error|free|malloc)'

proctab.c:	proc.c token.h awk.h
		${HOST_CC} -std=gnu89 -Wall -o proc proc.c
		./proc > proctab.c

clean:
		-rm -f *.o *.tmp awk.lx.c *.out *.elf *.dis y.tab.* awk.h proc proctab.c token.h awk
