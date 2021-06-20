# The "PROG" variable MUST be set in all Makefiles that include this Makefile.

QEMUOPTS=	-board STM32F4-Discovery # Target board
QEMUOPTS+=	--mcu STM32F407VG	# CPU on target board
QEMUOPTS+=	-d unimp,guest_errors	# Debugging options for QEMU
QEMUOPTS+=	-icount shift=1		# Speed up or slow down emulation
QEMUOPTS+=	--semihosting-config enable=on,target=native	# Semihosting
QEMUOPTS+=	-verbose -verbose	# Show info about CPU and board used
QEMUOPTS+=	-s			# Shorthand for '-gdb tcp::1234'
QEMUOPTS+=	-S			# Do not start CPU at startup (for gdb)
QEMUOPTS+=	--nographic		# Do not show circuit board

all: target_info

target_info:
	@echo "Targets are:" 1>&2
	@echo "    build, clean, flash, qemu, gdb-qemu, ocd, gdb-ocd" 1>&2
	@echo "" 1>&2
	@echo "Run 'make build' to compile binary" 1>&2
	@echo "" 1>&2
	@echo "Run 'make clean' to delete binary" 1>&2
	@echo "" 1>&2
	@echo "Run 'make flash' to upload binary to board" 1>&2
	@echo "" 1>&2
	@echo "=== Debugging binary in Emulator ===" 1>&2
	@echo "Run 'make qemu' in one terminal" 1>&2
	@echo "Run 'make gdb-qemu' in another terminal" 1>&2
	@echo "Enter 'c' in gdb to run the binary" 1>&2
	@echo "" 1>&2
	@echo "=== Debugging binary on Circuit Board ===" 1>&2
	@echo "Run 'make ocd' in one terminal" 1>&2
	@echo "Run 'make gdb-ocd' in another terminal" 1>&2
	@echo "Enter 'mon reset halt' in gdb to reset the cpu" 1>&2
	@echo "Enter 'load' in gdb to upload the elf file" 1>&2
	@echo "Enter 'c' in gdb to run the elf file" 1>&2

build: ${PROG}.elf

${PROG}.elf: ${PROG}.S
	arm-none-eabi-gcc -g -x assembler-with-cpp -Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -mno-thumb-interwork --specs=nosys.specs -nostdlib -mfloat-abi=soft -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -Wl,-gc-sections -Wl,-Map,${PROG}.map -T../stm32f407vg.ld ${PROG}.S -o $@

${PROG}.bin: ${PROG}.elf
	arm-none-eabi-objcopy -O binary ${PROG}.elf ${PROG}.bin

flash: ${PROG}.bin
	st-flash --reset write ${PROG}.bin 0x08000000

ocd:
	openocd -f "board/stm32f4discovery.cfg"

gdb-ocd: ${PROG}.elf
	arm-none-eabi-gdb -q \
		-ex 'target remote :3333' \
		-ex 'mon reset halt' \
		-ex 'load' \
		${PROG}.elf

qemu: ${PROG}.elf
	qemu-system-gnuarmeclipse ${QEMUOPTS} -image ${PROG}.elf

gdb-qemu: ${PROG}.elf
	arm-none-eabi-gdb -q -ex 'target remote :1234' ${PROG}.elf

clean:
	rm -f ${PROG}.bin ${PROG}.elf ${PROG}.map *.o *.core

.PHONY: all flash ocd gdb-ocd qemu gdb-qemu clean target_info
