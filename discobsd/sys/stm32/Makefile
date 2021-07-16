
# Programs that live in subdirectories, and have makefiles of their own.
#
SUBDIR		= f469idisco f4discovery

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

configsys:
		-for i in $(SUBDIR); do echo $$i; \
                I=`echo $$i | awk '{print toupper($$0)}'`; \
                if [ ! -f $$i/Config ]; then \
                (cd $$i; ../../../tools/configsys/config $$I); fi; done
