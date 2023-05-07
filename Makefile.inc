#BOARD?=		f411renucleo
BOARD?=		f412gdisco
#BOARD?=		f469idisco
#BOARD?=		f4discovery
KERNDIR=	$(TOPSRC)/sys/stm32/$(BOARD)
KERN=		unix

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
	cd ${KERNDIR}
ifeq ($(BOARD), f411renucleo)
	sudo openocd -f "board/st_nucleo_f4.cfg"
endif
ifeq ($(BOARD), f412gdisco)
	sudo openocd -f "board/stm32f412g-disco.cfg"
endif
ifeq ($(BOARD), f469idisco)
	sudo openocd -f "board/stm32f469discovery.cfg"
endif
ifeq ($(BOARD), f4discovery)
	sudo openocd -f "board/stm32f4discovery.cfg"
endif

gdb-ocd: kernel
	cd ${KERNDIR}
	arm-none-eabi-gdb -q \
		-ex 'cd ${KERNDIR}' \
		-ex 'target extended-remote :3333' \
		-ex 'mon reset halt' \
		-ex 'load' \
		${KERNDIR}/${KERN}

.PHONY: help ocd gdb-ocd
