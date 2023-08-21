SUBDIR!=if [ x"${MACHINE}" = x"pic32" ] ; then \
		echo "aoututils elf2aout fsutil kconfig icache mkrd virtualmips" ; \
	else \
		echo "aoututils elf2aout fsutil kconfig" ; \
	fi

all install depend: ${SUBDIR}
		-for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} $@; done

clean:
		rm -f *~ bin/*
		for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} clean; done
