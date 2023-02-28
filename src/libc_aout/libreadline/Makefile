TOPSRC		= $(shell cd ../../..; pwd)
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/src/libreadline

CFLAGS		= -Os $(DEFS) -Wall -Werror

ifeq ($(MACHINE_ARCH), mips)
CFLAGS		+= -Wa,-x -B$(TOPSRC)/src/libc_aout/
endif

OBJS		= readline.o

all:            ../libreadline.a

../libreadline.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libreadline.a
		${INSTALLDIR} ${DESTDIR}/lib
		${INSTALL} ../libreadline.a ${DESTDIR}/lib/libreadline.a

clean:
		rm -f *~ *.o a.out *.a
