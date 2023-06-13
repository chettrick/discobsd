TOPSRC!=cd ../../..; pwd
include $(TOPSRC)/target.mk

LIBDIR=		${TOPSRC}/lib/libm

COPTS!=if [ x"${MACHINE_ARCH}" = x"mips" ] ; then \
		echo "-Wa,-x -B${TOPSRC}/lib/libc_aout/" ; \
	else \
		echo "" ; \
	fi

CFLAGS          = -Os ${DEFS} -Wall -Werror ${COPTS}

OBJS            = asin.o atan.o exp.o erf.o floor.o fmod.o hypot.o j0.o j1.o \
                  jn.o log.o pow.o sin.o sinh.o sqrt.o tan.o tanh.o

all:            ../libm.a

%.o:		${LIBDIR}/%.c
		${CC} ${DEFS} ${CFLAGS} -c $< -o $@

../libm.a: ../ar ../ranlib $(OBJS)
		../ar rc $@ $(OBJS)
		../ranlib $@

install:	../libm.a
		${INSTALLDIR} ${DESTDIR}/usr/lib
		${INSTALL} ../libm.a ${DESTDIR}/usr/lib/libm.a
		../ranlib -t ${DESTDIR}/usr/lib/libm.a

clean:
		rm -f *~ *.o a.out *.a ../libm.a


asin.o:		${LIBDIR}/asin.c
atan.o:		${LIBDIR}/atan.c
exp.o:		${LIBDIR}/exp.c
erf.o:		${LIBDIR}/erf.c
floor.o:	${LIBDIR}/floor.c
fmod.o:		${LIBDIR}/fmod.c
hypot.o:	${LIBDIR}/hypot.c
j0.o:		${LIBDIR}/j0.c
j1.o:		${LIBDIR}/j1.c
jn.o:		${LIBDIR}/jn.c
log.o:		${LIBDIR}/log.c
pow.o:		${LIBDIR}/pow.c
sin.o:		${LIBDIR}/sin.c
sinh.o:		${LIBDIR}/sinh.c
sqrt.o:		${LIBDIR}/sqrt.c
tan.o:		${LIBDIR}/tan.c
tanh.o:		${LIBDIR}/tanh.c
