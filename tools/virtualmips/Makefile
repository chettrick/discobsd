TOPSRC!=cd ../..; pwd

-include ../Makefile.inc

#
# Build VirtualMIPS simulator.
#

# Select target board:
# chipKIT Max32
CFLAGS		= -DSIM_PIC32 -DPIC32MX7 -DMAX32

# Maximite
#CFLAGS		= -DSIM_PIC32 -DPIC32MX7 -DMAXIMITE

# Microchip Explorer 16
#CFLAGS		= -DSIM_PIC32 -DPIC32MX7 -DEXPLORER16

# UBW32 with UART console
#CFLAGS		= -DSIM_PIC32 -DPIC32MX7 -DUBW32

#CFLAGS		+= -Wall -MT $@ -MD -MP -MF .deps/$*.dep -I/opt/local/include -I/opt/local/include/libelf
CFLAGS		+= -Wall -MT $@ -MD -MP -MF .deps/$*.dep -I/usr/local/include -I/usr/local/include/elftoolchain
LIBS		= -lpthread -lelf

LIBS		+= -L/usr/local/lib/elftoolchain

# libbsd-dev package on Linux in overlay mode.
CFLAGS+=	${_LIBBSD_CFLAGS}
LIBS+=		${_LIBBSD_LIBS}

ADDLIBS!=if [ x"${_HOST_OSNAME}" = x"OpenBSD" ] ; then \
		echo "-L/usr/local/lib -lintl" ; \
	elif [ x"${_HOST_OSNAME}" = x"Darwin" ] ; then \
		echo "-L/opt/local/lib -lintl" ; \
	elif [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
		echo "-lrt" ; \
	else \
		echo "" ; \
	fi

LIBS		+= ${ADDLIBS}

# Optimization.
CFLAGS		+= -O
#CFLAGS		+= -O3 -fomit-frame-pointer

# Microchip PIC32MX
PROG		= pic32
OBJS		= pic32.o pic32_dev_flash.o pic32_dev_uart.o pic32_dev_intcon.o \
              pic32_dev_spi.o pic32_dev_gpio.o dev_sdcard.o dev_swap.o \
              pic32_dev_bmxcon.o pic32_dev_dmacon.o pic32_dev_syscon.o \
              pic32_dev_prefetch.o pic32_dev_adc.o pic32_dev_devcfg.o \
              pic32_dev_rtcc.o pic32_dev_timer.o

# Generic M4K core
#PROG		= m4k
#OBJS		= m4k.o

# Ingenic JZ4740.
#CFLAGS		+= -DSIM_PAVO -D_USE_FDD_
#PROG		= pavo
#OBJS		= pavo.o jz4740.o jz4740_dev_cpm.o jz4740_dev_dma.o \
#              jz4740_dev_emc.o jz4740_dev_gpio.o jz4740_dev_int.o \
#              jz4740_dev_rtc.o jz4740_dev_ts.o \
#              jz4740_dev_uart.o jz4740_dev_wdt_tcu.o

# Implement LCD.
#CFLAGS		+= -DSIM_LCD
#LIBS		+= -lSDL
#OBJS		+= jz4740_dev_lcd.o vp_sdl.o

# Use JIT compiler.
#CFLAGS		+= -D_USE_JIT_
#OBJS		+= mips_jit.o x86_trans.o

# Common files.
OBJS		+= dev_cs8900.o dev_nand_flash_1g.o dev_ram.o \
               dev_vtty.o device.o vm.o cpu.o mips.o mips_cp0.o \
               mips_exec.o mips_fdd.o crc.o mips_hostalarm.o \
               mempool.o sbox.o utils.o vp_clock.o vp_timer.o net_io.o \
               mips_memory.o debug.o gdb_interface.o main.o \
               mips-dis.o config.o

all:		.deps $(PROG)

$(PROG):	$(OBJS)
			$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.deps:
			@mkdir .deps

install:	all

clean:
			rm -rf *.o *~ *_log.txt .deps pavo m4k pic32 pic32-log.txt

-include .deps/*.dep
