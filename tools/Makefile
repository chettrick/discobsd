SUBDIR!=if [ x"${MACHINE}" = x"pic32" ] ; then \
		echo "binstall aoututils elf2aout fsutil config icache mkrd virtualmips" ; \
	else \
		echo "binstall aoututils elf2aout fsutil config" ; \
	fi

all install depend: ${SUBDIR}
		-for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} $@; done

clean:
		rm -f *~ bin/*
		for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} clean; done
