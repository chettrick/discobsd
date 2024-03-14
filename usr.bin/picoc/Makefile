TOPSRC!=cd ../..; pwd
include ${TOPSRC}/share/mk/sys.mk

VPATH           = cstdlib platform

CFLAGS          += -DUNIX_HOST -DVER='"1.0"' -DFILENAME_MAX=64 \
                   -DL_tmpnam=30 -DCLOCKS_PER_SEC=80000000 \
                   -DNO_FP -Os

LIBS            = -lm -lc

TARGET          = picoc

OBJS            = picoc.o table.o lex.o parse.o expression.o heap.o type.o \
                  variable.o clibrary.o platform.o include.o debug.o \
                  platform_unix.o library_unix.o \
                  stdio.o math.o string.o stdlib.o \
                  time.o errno.o ctype.o stdbool.o \
                  unistd.o retrobsd.o

all:            $(TARGET)

$(TARGET):      $(OBJS)
		${CC} ${CFLAGS} ${LDFLAGS} -o ${TARGET}.elf ${OBJS} ${LIBS}
		${OBJDUMP} -S ${TARGET}.elf > ${TARGET}.dis
		${SIZE} ${TARGET}.elf
		${ELF2AOUT} ${TARGET}.elf $@

install:        $(TARGET)
		${INSTALL} ${TARGET} ${DESTDIR}/usr/bin/

test:           all
		(cd tests; make test)

clean:
		rm -f $(TARGET) *.o */*.o *~ *.elf *.dis

picoc.o: picoc.c picoc.h
table.o: table.c interpreter.h platform.h
lex.o: lex.c interpreter.h platform.h
parse.o: parse.c picoc.h interpreter.h platform.h
expression.o: expression.c interpreter.h platform.h
heap.o: heap.c interpreter.h platform.h
type.o: type.c interpreter.h platform.h
variable.o: variable.c interpreter.h platform.h
clibrary.o: clibrary.c picoc.h interpreter.h platform.h
platform.o: platform.c picoc.h interpreter.h platform.h
include.o: include.c picoc.h interpreter.h platform.h
debug.o: debug.c interpreter.h platform.h
platform_unix.o: platform/platform_unix.c picoc.h interpreter.h platform.h
library_unix.o: platform/library_unix.c interpreter.h platform.h
stdio.o: cstdlib/stdio.c interpreter.h platform.h
math.o: cstdlib/math.c interpreter.h platform.h
string.o: cstdlib/string.c interpreter.h platform.h
stdlib.o: cstdlib/stdlib.c interpreter.h platform.h
time.o: cstdlib/time.c interpreter.h platform.h
errno.o: cstdlib/errno.c interpreter.h platform.h
ctype.o: cstdlib/ctype.c interpreter.h platform.h
stdbool.o: cstdlib/stdbool.c interpreter.h platform.h
unistd.o: cstdlib/unistd.c interpreter.h platform.h
