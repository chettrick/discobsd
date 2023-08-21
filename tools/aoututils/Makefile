#
# aoututils - utilities for building and maintaining a.out executables
#
# Must be kept in sync with utilities in usr.bin
#

SUBDIR=	aout ar as ld nm ranlib size strip

all install clean:
	for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} $@; done
