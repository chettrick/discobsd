#BOARD?=		f411renucleo
BOARD?=		f412gdisco
#BOARD?=		f412weactcore
#BOARD?=		f469idisco
#BOARD?=		f4discovery
KERNDIR=	${TOPSRC}/sys/stm32/${BOARD}
KERN=		unix

CONFSCRIPT!=if [ x"${BOARD}" = x"f405weactcore" ] ; then \
		echo "board/st_nucleo_f4.cfg" ; \
	elif [ x"${BOARD}" = x"f411renucleo" ] ; then \
		echo "board/st_nucleo_f4.cfg" ; \
	elif [ x"${BOARD}" = x"f412gdisco" ] ; then \
		echo "board/stm32f412g-disco.cfg" ; \
	elif [ x"${BOARD}" = x"f412weactcore" ] ; then \
		echo "board/st_nucleo_f4.cfg" ; \
	elif [ x"${BOARD}" = x"f469idisco" ] ; then \
		echo "board/stm32f469i-disco.cfg" ; \
	elif [ x"${BOARD}" = x"f4discovery" ] ; then \
		echo "board/stm32f4discovery.cfg" ; \
	else \
		echo "does/not/exist" ; \
	fi

help:
	@echo "Targets are:" 1>&2
	@echo "    ocd, gdb-ocd" 1>&2
	@echo "" 1>&2
	@echo "=== Debugging elf file in Circuit Board ===" 1>&2
	@echo "Run 'make ocd' in one terminal" 1>&2
	@echo "Run 'make gdb-ocd' in another terminal" 1>&2
	@echo "Enter 'c' in gdb to run the elf file" 1>&2
	@echo "Enter 'mon reset halt' in gdb to reset the cpu" 1>&2
	@echo "Enter 'load' in gdb to upload the elf file" 1>&2

ocd: kernel
	sudo openocd -f "${CONFSCRIPT}"

gdb-ocd: kernel
	arm-none-eabi-gdb -q \
		-ex 'cd ${KERNDIR}' \
		-ex 'target extended-remote :3333' \
		-ex 'mon reset halt' \
		-ex 'load' \
		${KERNDIR}/${KERN}

.PHONY: help ocd gdb-ocd
