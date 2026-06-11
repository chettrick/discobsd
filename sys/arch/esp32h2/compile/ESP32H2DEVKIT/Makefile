#
# Makefile -- DiscoBSD/esp32h2 Phase 1 build.
#

CROSS_COMPILE ?= riscv64-unknown-elf-

CC      = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE    = $(CROSS_COMPILE)size

TOPDIR  = ../..
MACHDIR = $(TOPDIR)/esp32h2
INCDIR  = $(TOPDIR)/include

MARCH   = -march=rv32imac -mabi=ilp32

CFLAGS  = $(MARCH)                  \
          -Os                       \
          -ffreestanding            \
          -fno-builtin              \
          -fno-stack-protector      \
          -fcommon                  \
          -Wall                     \
          -Wno-unused-function      \
          -I$(INCDIR)               \
          -DCONFIG_CPU_FREQ_MHZ=96  \
          -DHZ=100

ASFLAGS = $(MARCH)                  \
          -x assembler-with-cpp     \
          -I$(INCDIR)

LDFLAGS = $(MARCH)                  \
          -nostdlib                 \
          -Wl,--gc-sections         \
          -T esp32h2.ld

SRCS =  $(MACHDIR)/locore.S     \
        $(MACHDIR)/uart.c       \
        $(MACHDIR)/machdep.c    \
        $(MACHDIR)/main.c

OBJS =  locore.o    \
        uart.o      \
        machdep.o   \
        main.o

.PHONY: all flash info clean size

all: unix.elf unix.bin
	@echo ""
	@$(SIZE) unix.elf
	@echo ""
	@echo "Flash:   make flash PORT=/dev/ttyUSB0"
	@echo "Connect: screen /dev/ttyUSB0 115200"

unix.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

unix.bin: unix.elf
	$(OBJCOPY) -O binary $< $@

locore.o: $(MACHDIR)/locore.S
	$(CC) $(ASFLAGS) -c -o $@ $<

uart.o: $(MACHDIR)/uart.c
	$(CC) $(CFLAGS) -c -o $@ $<

machdep.o: $(MACHDIR)/machdep.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: $(MACHDIR)/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

PORT  ?= /dev/ttyUSB0
BAUD  ?= 921600

# Show ELF segments and generated image info (useful for debugging)
info: unix.elf unix.bin
	@echo "=== ELF segments ==="
	$(OBJDUMP) -h unix.elf
	@echo ""
	@echo "=== elf2image output ==="
	esptool --chip esp32h2 elf2image \
	    --flash-mode dio                \
	    --flash-freq 48m                \
	    --flash-size 4MB                \
	    -o unix.bin unix.elf
	esptool image-info unix.bin

flash: unix.elf
	esptool --chip esp32h2 elf2image \
	    --flash-mode dio                \
	    --flash-freq 48m                \
	    --flash-size 4MB                \
	    -o unix.bin unix.elf
	esptool --chip esp32h2           \
	    --port $(PORT)                  \
	    --baud $(BAUD)                  \
	    write_flash 0x0 unix.bin

size: unix.elf
	$(SIZE) -A $<

clean:
	rm -f $(OBJS) unix.elf unix.bin
