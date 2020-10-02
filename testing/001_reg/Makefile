PROG=		001_reg

QEMUOPTS=	-board STM32F4-Discovery # Target board
QEMUOPTS+=	--mcu STM32F407VG	# CPU on target board
QEMUOPTS+=	-d unimp,guest_errors	# Debugging options for QEMU
QEMUOPTS+=	-icount shift=1		# Speed up or slow down emulation
QEMUOPTS+=	--semihosting-config enable=on,target=native	# Semihosting
QEMUOPTS+=	-verbose -verbose	# Show info about CPU and board used
QEMUOPTS+=	-s			# Shorthand for '-gdb tcp::1234'
QEMUOPTS+=	-S			# Do not start CPU at startup (for gdb)
#QEMUOPTS+=	--nographic		# Do not show circuit board

all:
	@echo "Targets are: prog, qemu, gdb, flash, and clean" 1>&2
	@echo "" 1>&2
	@echo "Run 'make prog' to compile binary" 1>&2
	@echo "" 1>&2
	@echo "Run 'make qemu' in one terminal" 1>&2
	@echo "Run 'make gdb' in another terminal" 1>&2
	@echo "Press 'c' in gdb to run the binary" 1>&2
	@echo "" 1>&2
	@echo "Run 'make flash' to upload binary to board" 1>&2
	@echo "" 1>&2
	@echo "Run 'make clean' to delete binary" 1>&2

prog: ${PROG}.elf

${PROG}.elf: ${PROG}.S
	arm-none-eabi-gcc -g -x assembler-with-cpp -Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -mno-thumb-interwork --specs=nosys.specs -nostdlib -mfloat-abi=soft -mfpu=fpv4-sp-d16 -ffunction-sections -fdata-sections -Wl,-gc-sections -Wl,-Map,${PROG}.map -Tstm32f407vg.ld ${PROG}.S -o $@

${PROG}.bin: ${PROG}.elf
	arm-none-eabi-objcopy -O binary ${PROG}.elf ${PROG}.bin

flash: ${PROG}.bin
	st-flash --reset write ${PROG}.bin 0x08000000

gdb: ${PROG}.elf
	arm-none-eabi-gdb -q -ex 'target remote :1234' ${PROG}.elf

qemu: ${PROG}.elf
	qemu-system-gnuarmeclipse ${QEMUOPTS} -image ${PROG}.elf

clean:
	rm -f ${PROG}.bin ${PROG}.elf ${PROG}.map *.o *.core

.PHONY: all flash qemu gdb clean
