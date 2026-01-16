#
# Kernels live in subdirectories, and have Makefiles of their own.
#
SUBDIR=		F405WEACTCORE F411RENUCLEO F412GDISCO F412WEACTCORE \
		F413HDISCO F469IDISCO F4DISCOVERY

default:

all:
		-for i in ${SUBDIR}; do ${MAKE} -C $$i all; done

install:

clean:
		-for i in ${SUBDIR}; do ${MAKE} -C $$i clean; done

clean-all:
		-for i in ${SUBDIR}; do ${MAKE} -C $$i clean-all; done

reconfig:
		-for i in ${SUBDIR}; do ${MAKE} -C $$i reconfig; done
