#
# Kernels live in subdirectories, and have Makefiles of their own.
#
SUBDIR=		F405WEACTCORE F411RENUCLEO F412GDISCO F412WEACTCORE \
		F413HDISCO F446RENUCLEO F446WEACTCORE F469IDISCO \
		F4DISCOVERY F4MDEVEBOX F4VEDEVEBOX

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
