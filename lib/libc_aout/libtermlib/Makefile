TOPSRC		= $(shell cd ../../..; pwd)
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/lib/libtermlib

CFLAGS		= -Os $(DEFS) -Wall -Werror

ifeq ($(MACHINE_ARCH), mips)
CFLAGS		+= -Wa,-x -B$(TOPSRC)/lib/libc_aout/
endif

OBJS		= termcap.o tgoto.o tputs.o tcattr.o

all:            ../libtermlib.a

../libtermlib.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libtermlib.a
		${INSTALLDIR} ${DESTDIR}/lib
		${INSTALL} ../libtermlib.a ${DESTDIR}/lib/libtermlib.a

clean:
		rm -f *~ *.o a.out *.a
