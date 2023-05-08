SUBDIR!=if [ x"${MACHINE}" = x"pic32" ] ; then \
		echo "elf2aout fsutil kconfig icache mkrd virtualmips" ; \
	else \
		echo "elf2aout fsutil kconfig" ; \
	fi

all install depend: ${SUBDIR}
		-for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} DESTDIR=${DESTDIR} $@; done

clean:
		rm -f *~
		for i in ${SUBDIR}; do ${MAKE} -C $$i ${MFLAGS} clean; done
