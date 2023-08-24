#
# libc_aout - a.out libraries, built with a.out utilities
#

SUBDIR=	startup libc libm libcurses libtermlib \
	libreadline libgpanel libwiznet

all install:
	-for dir in ${SUBDIR}; do \
		${MAKE} -C $$dir ${MFLAGS} $@; \
	done

clean:
	rm -f *~ *.o *.a
	for dir in ${SUBDIR}; do \
		${MAKE} -C $$dir ${MFLAGS} clean; \
	done
