#
# Kernels live in subdirectories, and have Makefiles of their own.
#
SUBDIR=		f405weactcore f411renucleo f412gdisco f412weactcore \
		f469idisco f4discovery

default:

all:
		-for i in $(SUBDIR); do ${MAKE} -C $$i all; done

install:

clean:
		-for i in $(SUBDIR); do ${MAKE} -C $$i clean; done

clean-all:
		-for i in $(SUBDIR); do ${MAKE} -C $$i clean-all; done

reconfig:
		-for i in $(SUBDIR); do ${MAKE} -C $$i reconfig; done
