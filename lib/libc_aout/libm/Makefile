TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/lib/libm

CFLAGS          = -Os $(DEFS) -Wall -Werror

ifeq ($(MACHINE_ARCH), mips)
CFLAGS          += -Wa,-x -B$(TOPSRC)/lib/libc_aout/
endif

OBJS            = asin.o atan.o exp.o erf.o floor.o fmod.o hypot.o j0.o j1.o \
                  jn.o log.o pow.o sin.o sinh.o sqrt.o tan.o tanh.o

all:            ../libm.a

../libm.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libm.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libm.a ${DESTDIR}/usr/lib/libm.a

clean:
		rm -f *~ *.o a.out *.a
