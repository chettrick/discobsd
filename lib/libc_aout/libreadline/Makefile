TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/lib/libreadline

CFLAGS		= -Os $(DEFS) -Wall -Werror

ifeq ($(MACHINE_ARCH), mips)
CFLAGS		+= -Wa,-x -B$(TOPSRC)/lib/libc_aout/
endif

OBJS		= readline.o

all:            ../libreadline.a

../libreadline.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libreadline.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libreadline.a ${DESTDIR}/usr/lib/libreadline.a

clean:
		rm -f *~ *.o a.out *.a
