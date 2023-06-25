TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

LIBDIR=		${TOPSRC}/lib/libgpanel

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS		= -Os ${DEFS} -Wall -Werror ${COPTS}

OBJS            = open.o clear.o pixel.o line.o rect.o fill.o fill_triangle.o \
                  circle.o image.o char.o text.o text_width.o

all:            ../libgpanel.a

%.o:		${LIBDIR}/%.c
		${CC} ${DEFS} ${CFLAGS} -c $< -o $@

../libgpanel.a:	${OBJS}
		../ar_aout rc $@ ${OBJS}
		../ranlib_aout $@

install:	../libgpanel.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libgpanel.a ${DESTDIR}/usr/lib/libgpanel.a
		../ranlib_aout -t ${DESTDIR}/usr/lib/libgpanel.a

clean:
		rm -f *~ *.o a.out *.a ../libgpanel.a


open.o:		${LIBDIR}/open.c
clear.o:	${LIBDIR}/clear.c
pixel.o:	${LIBDIR}/pixel.c
line.o:		${LIBDIR}/line.c
rect.o:		${LIBDIR}/rect.c
fill.o:		${LIBDIR}/fill.c
fill_triangle.o:${LIBDIR}/fill_triangle.c
circle.o:	${LIBDIR}/circle.c
image.o:	${LIBDIR}/image.c
char.o:		${LIBDIR}/char.c
text.o:		${LIBDIR}/text.c
text_width.o:	${LIBDIR}/text_width.c
