#BOARD?=		f411renucleo
BOARD?=		f412gdisco
#BOARD?=		f469idisco
#BOARD?=		f4discovery
KERNDIR=	$(TOPSRC)/sys/stm32/$(BOARD)
KERN=		unix

help:
	@echo "Targets are:" 1>&2
	@echo "    flash, qemu, gdb-qemu, ocd, gdb-ocd" 1>&2
	@echo "" 1>&2
	@echo "Run 'make flash' to upload binary to board" 1>&2
	@echo "" 1>&2
	@echo "=== Debugging binary in Emulator ===" 1>&2
	@echo "Run 'make qemu' in one terminal" 1>&2
	@echo "Run 'make gdb-qemu' in another terminal" 1>&2
	@echo "Enter 'c' in gdb to run the binary" 1>&2
	@echo "" 1>&2
	@echo "=== Debugging elf file in Circuit Board ===" 1>&2
	@echo "Run 'make ocd' in one terminal" 1>&2
	@echo "Run 'make gdb-ocd' in another terminal" 1>&2
	@echo "Enter 'c' in gdb to run the elf file" 1>&2
	@echo "Enter 'mon reset halt' in gdb to reset the cpu" 1>&2
	@echo "Enter 'load' in gdb to upload the elf file" 1>&2

flash: kernel
	st-flash --reset write ${KERNDIR}/${KERN}.bin 0x08000000

qemu: kernel
ifeq ($(BOARD), f4discovery)
	cd ${KERNDIR}
	qemu-system-gnuarmeclipse \
		-board STM32F4-Discovery \
		--mcu STM32F407VG \
		-d unimp,guest_errors \
		-icount shift=1 \
		--semihosting-config enable=on,target=native \
		-verbose -verbose \
		-s \
		-S \
		-image ${KERNDIR}/${KERN}
#		--nographic
else
	@echo "Board ${BOARD} is not supported by qemu" 1>&2
endif

gdb-qemu: kernel
	cd ${KERNDIR}
	arm-none-eabi-gdb -q \
		-ex 'cd ${KERNDIR}' \
		-ex 'target remote :1234' \
		${KERNDIR}/${KERN}

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

.PHONY: help flash ocd gdb-ocd qemu gdb-qemu
