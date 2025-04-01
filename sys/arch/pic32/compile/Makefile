#
# Kernels live in subdirectories, and have Makefiles of their own.
#
SUBDIR=		BAREMETAL DUINOMITE DUINOMITE_EMEGA EXPLORER16 \
		FUBARINO MAX32 MAXIMITE MAXIMITE_COLOR MMB_MX7 \
		PICADILLO PINGUINO_MICRO QUICK240 \
		SDXL SNADPIC STARTER_KIT UBW32 WF32

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
