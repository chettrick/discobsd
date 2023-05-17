TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

vpath %.c $(TOPSRC)/lib/libgpanel

CFLAGS		= -Os $(DEFS) -Wall -Werror

ifeq ($(MACHINE_ARCH), mips)
CFLAGS		+= -Wa,-x -B$(TOPSRC)/lib/libc_aout/
endif

OBJS            = open.o clear.o pixel.o line.o rect.o fill.o fill_triangle.o \
                  circle.o image.o char.o text.o text_width.o

all:            ../libgpanel.a

../libgpanel.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libgpanel.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libgpanel.a ${DESTDIR}/usr/lib/libgpanel.a

clean:
		rm -f *~ *.o a.out *.a
