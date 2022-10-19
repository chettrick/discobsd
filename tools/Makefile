ifeq ($(MACHINE), pic32)
SUBDIR		= elf2aout fsutil virtualmips mkrd kconfig icache
else
SUBDIR		= elf2aout fsutil mkrd kconfig icache
endif

all install depend: ${SUBDIR}
		-for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} DESTDIR=${DESTDIR} $@; done

clean:
		rm -f *~
		for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} clean; done
