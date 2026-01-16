#BOARD?=		F405WEACTCORE
#BOARD?=		F411RENUCLEO
BOARD?=		F412GDISCO
#BOARD?=		F412WEACTCORE
#BOARD?=		F413HDISCO
#BOARD?=		F469IDISCO
#BOARD?=		F4DISCOVERY
KERNDIR=	${TOPSRC}/sys/arch/stm32/compile/${BOARD}
KERN=		unix

CONFSCRIPT!=if [ x"${BOARD}" = x"F405WEACTCORE" ] ; then \
		echo "board/st_nucleo_f4.cfg" ; \
	elif [ x"${BOARD}" = x"F411RENUCLEO" ] ; then \
		echo "board/st_nucleo_f4.cfg" ; \
	elif [ x"${BOARD}" = x"F412GDISCO" ] ; then \
		echo "board/stm32f412g-disco.cfg" ; \
	elif [ x"${BOARD}" = x"F412WEACTCORE" ] ; then \
		echo "board/st_nucleo_f4.cfg" ; \
	elif [ x"${BOARD}" = x"F413HDISCO" ] ; then \
		echo "board/stm32f413h-disco.cfg" ; \
	elif [ x"${BOARD}" = x"F469IDISCO" ] ; then \
		echo "board/stm32f469i-disco.cfg" ; \
	elif [ x"${BOARD}" = x"F4DISCOVERY" ] ; then \
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

ocd:
	sudo openocd -f "${CONFSCRIPT}"

gdb-ocd:
	arm-none-eabi-gdb -q \
		-ex 'cd ${KERNDIR}' \
		-ex 'target extended-remote :3333' \
		-ex 'mon reset halt' \
		-ex 'load' \
		${KERNDIR}/${KERN}

.PHONY: help ocd gdb-ocd
