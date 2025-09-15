PARAM = -DDUINOMITE
PARAM += -DPIC32MX7
PARAM += -DUARTUSB_ENABLED
PARAM += -DSPI3_ENABLED
PARAM += -DSD_ENABLED
PARAM += -DGPIO1_ENABLED
PARAM += -DGPIO2_ENABLED
PARAM += -DGPIO3_ENABLED
PARAM += -DGPIO4_ENABLED
PARAM += -DGPIO5_ENABLED
PARAM += -DGPIO6_ENABLED
PARAM += -DADC_ENABLED
PARAM += -DPWM_ENABLED
PARAM += -DGPANEL_ENABLED
PARAM += -DLCD_D1_PORT=TRISB -DLCD_D1_PIN=12
PARAM += -DLCD_D0_PORT=TRISB -DLCD_D0_PIN=11
PARAM += -DLCD_D7_PORT=TRISE -DLCD_D7_PIN=7
PARAM += -DLCD_D6_PORT=TRISE -DLCD_D6_PIN=6
PARAM += -DLCD_D5_PORT=TRISE -DLCD_D5_PIN=5
PARAM += -DLCD_D4_PORT=TRISE -DLCD_D4_PIN=4
PARAM += -DLCD_D3_PORT=TRISE -DLCD_D3_PIN=3
PARAM += -DLCD_D2_PORT=TRISE -DLCD_D2_PIN=2
PARAM += -DLCD_RD_PORT=TRISB -DLCD_RD_PIN=3
PARAM += -DLCD_WR_PORT=TRISB -DLCD_WR_PIN=4
PARAM += -DLCD_RS_PORT=TRISB -DLCD_RS_PIN=6
PARAM += -DLCD_CS_PORT=TRISB -DLCD_CS_PIN=7
PARAM += -DLCD_RST_PORT=TRISB -DLCD_RST_PIN=9
PARAM += -DSD0_ENA_PORT=TRISB -DSD0_ENA_PIN=13
PARAM += -DLED_DISK_PORT=TRISB -DLED_DISK_PIN=12
PARAM += -DLED_KERNEL_PORT=TRISB -DLED_KERNEL_PIN=15
PARAM += -DUSB_NUM_STRING_DESCRIPTORS=3
PARAM += -DUSB_MAX_EP_NUMBER=3
PARAM += -DCONS_MAJOR=UARTUSB_MAJOR
PARAM += -DBUS_DIV=1
PARAM += -DBUS_KHZ=80000
PARAM += -DCPU_KHZ=80000
LDSCRIPT = "MAXIMITE.ld"
#
# Makefile for RetroBSD, pic32 target
#
# DEBUG is set to -g by kconfig if debugging is requested (kconfig -g).
#

AS              = ${MIPS_GCC_PREFIX}as ${DEBUG} -mips32r2 -EL
CC              = ${MIPS_GCC_PREFIX}gcc ${DEBUG} -mips32r2 -EL -nostdinc -fno-builtin -fcommon -Wall
CPP             = ${MIPS_GCC_PREFIX}cpp
LD              = ${MIPS_GCC_PREFIX}gcc -mips32r2 -EL
LDFLAGS         = -nostdlib -T ${_confdir}/${LDSCRIPT} -Wl,-Map=$@.map
SIZE            = ${MIPS_GCC_PREFIX}size
OBJCOPY         = ${MIPS_GCC_PREFIX}objcopy
OBJDUMP         = ${MIPS_GCC_PREFIX}objdump

# sources are located via $S relative to the compilation directory
S               = ../../../..

_mach?=		pic32
_arch?=		mips
_machdir?=	$S/arch/${_mach}
_archdir?=	$S/arch/${_arch}
_confdir?=	${_machdir}/conf

include		${_confdir}/compiler.mk

INCLUDES=	-I. -I$S -I$S/arch
DEPFLAGS=	-MT $@ -MP -MD -MF .deps/$*.dep
DEFS=		${INCLUDES} ${PARAM} -DKERNEL ${DEPFLAGS}
CFLAGS=		-O ${DEFS}

# compile rules: rules are named COMPILE_${SUFFIX}
# SUFFIX is the file suffix, capitalized (e.g. C for a .c file).

COMPILE_C       = ${CC} -c ${CFLAGS} $<
COMPILE_S       = ${CC} -c ${DEFS} $<

OBJS = exec_aout.o exec_conf.o exec_elf.o exec_script.o exec_subr.o \
	init_main.o init_sysent.o kern_clock.o kern_descrip.o \
	kern_exec.o kern_exit.o kern_fork.o kern_mman.o kern_proc.o \
	kern_prot.o kern_prot2.o kern_resource.o kern_sig.o kern_sig2.o \
	kern_subr.o kern_synch.o kern_sysctl.o kern_time.o subr_prf.o \
	subr_rmap.o sys_generic.o sys_inode.o sys_pipe.o sys_process.o \
	syscalls.o tty.o tty_subr.o tty_tty.o ufs_alloc.o ufs_bio.o \
	ufs_bmap.o ufs_dsort.o ufs_fio.o ufs_inode.o ufs_mount.o \
	ufs_namei.o ufs_subr.o ufs_syscalls.o ufs_syscalls2.o \
	vfs_vnops.o vm_sched.o vm_swap.o vm_swp.o cons.o mem.o swap.o \
	clock.o conf.o exception.o machdep.o sig_machdep.o sysctl.o \
	usb_device.o usb_function_cdc.o adc.o gpanel-ili9341.o \
	gpanel-nt35702.o gpanel-st7781.o gpanel.o gpio.o pwm.o sd.o \
	spi.o spi_bus.o usb_uart.o 

CFILES = $S/kern/exec_aout.c $S/kern/exec_conf.c $S/kern/exec_elf.c \
	$S/kern/exec_script.c $S/kern/exec_subr.c $S/kern/init_main.c \
	$S/kern/init_sysent.c $S/kern/kern_clock.c \
	$S/kern/kern_descrip.c $S/kern/kern_exec.c $S/kern/kern_exit.c \
	$S/kern/kern_fork.c $S/kern/kern_mman.c $S/kern/kern_proc.c \
	$S/kern/kern_prot.c $S/kern/kern_prot2.c $S/kern/kern_resource.c \
	$S/kern/kern_sig.c $S/kern/kern_sig2.c $S/kern/kern_subr.c \
	$S/kern/kern_synch.c $S/kern/kern_sysctl.c $S/kern/kern_time.c \
	$S/kern/subr_prf.c $S/kern/subr_rmap.c $S/kern/sys_generic.c \
	$S/kern/sys_inode.c $S/kern/sys_pipe.c $S/kern/sys_process.c \
	$S/kern/syscalls.c $S/kern/tty.c $S/kern/tty_subr.c \
	$S/kern/tty_tty.c $S/kern/ufs_alloc.c $S/kern/ufs_bio.c \
	$S/kern/ufs_bmap.c $S/kern/ufs_dsort.c $S/kern/ufs_fio.c \
	$S/kern/ufs_inode.c $S/kern/ufs_mount.c $S/kern/ufs_namei.c \
	$S/kern/ufs_subr.c $S/kern/ufs_syscalls.c \
	$S/kern/ufs_syscalls2.c $S/kern/vfs_vnops.c $S/kern/vm_sched.c \
	$S/kern/vm_swap.c $S/kern/vm_swp.c $S/dev/cons.c $S/dev/mem.c \
	$S/dev/swap.c $S/arch/pic32/pic32/clock.c \
	$S/arch/pic32/pic32/conf.c $S/arch/pic32/pic32/exception.c \
	$S/arch/pic32/pic32/machdep.c $S/arch/pic32/pic32/sig_machdep.c \
	$S/arch/pic32/pic32/sysctl.c $S/arch/pic32/hal/usb_device.c \
	$S/arch/pic32/hal/usb_function_cdc.c $S/arch/pic32/dev/adc.c \
	$S/arch/pic32/dev/gpanel-ili9341.c \
	$S/arch/pic32/dev/gpanel-nt35702.c \
	$S/arch/pic32/dev/gpanel-st7781.c $S/arch/pic32/dev/gpanel.c \
	$S/arch/pic32/dev/gpio.c $S/arch/pic32/dev/pwm.c \
	$S/arch/pic32/dev/sd.c $S/arch/pic32/dev/spi.c \
	$S/arch/pic32/dev/spi_bus.c $S/arch/pic32/dev/usb_uart.c \
	swapunix.c 

DEVCFG!=if [ -f devcfg.c ] ; then \
		echo "devcfg.o" ; \
	else \
		echo "" ; \
	fi

# load lines for config "xxx" will be emitted as:
# xxx: ${SYSTEM_DEP} swapxxx.o
#	${SYSTEM_LD_HEAD}
#	${SYSTEM_LD} swapxxx.o
#	${SYSTEM_LD_TAIL}
SYSTEM_OBJ      = locore.o ${OBJS} ioconf.o ${DEVCFG}
SYSTEM_DEP      = Makefile ioconf.c machine sys .deps ${SYSTEM_OBJ}
SYSTEM_LD_HEAD  = sh $S/conf/newvers.sh > vers.c; ${CC} ${CFLAGS} -c vers.c; rm -f $@
SYSTEM_LD       = -@echo ${LD} ${LDFLAGS} '$${SYSTEM_OBJ}' vers.o -o $@; \
                  ${LD} ${LDFLAGS} ${SYSTEM_OBJ} vers.o -o $@
SYSTEM_LD_TAIL  = ${SIZE} $@; cp $@ $@.elf; \
                  $(OBJCOPY) -O ihex --change-addresses=0x80000000 $@ $@.hex; \
                  $(OBJCOPY) -O binary -R .boot -R .config $@ $@.bin; \
                  $(OBJDUMP) -d -S $@ > $@.dis

unix: ${SYSTEM_DEP} swapunix.o
	${SYSTEM_LD_HEAD}
	${SYSTEM_LD} swapunix.o
	${SYSTEM_LD_TAIL}

swapunix.o: swapunix.c
	${COMPILE_C}

all: unix

clean:
	rm -rf .deps unix *.elf *.o *.map *.dis *.bin *.hex machine sys

clean-all: clean
	rm -f *.h ioconf.c swap*.c vers.c

reconfig ioconf.c: Config $S/../tools/bin/kconfig
	$S/../tools/bin/kconfig Config
	$(MAKE) clean

load:   unix
	pic32prog unix.hex

machine:
	ln -s ${_machdir}/include $@

sys:
	ln -s $S/sys $@

.deps:
	mkdir .deps

locore.o: ${_machdir}/${_mach}/locore.S
	${COMPILE_S}

ioconf.o: ioconf.c
	${COMPILE_C}

exec_aout.o: $S/kern/exec_aout.c
	${COMPILE_C}

exec_conf.o: $S/kern/exec_conf.c
	${COMPILE_C}

exec_elf.o: $S/kern/exec_elf.c
	${COMPILE_C}

exec_script.o: $S/kern/exec_script.c
	${COMPILE_C}

exec_subr.o: $S/kern/exec_subr.c
	${COMPILE_C}

init_main.o: $S/kern/init_main.c
	${COMPILE_C}

init_sysent.o: $S/kern/init_sysent.c
	${COMPILE_C}

kern_clock.o: $S/kern/kern_clock.c
	${COMPILE_C}

kern_descrip.o: $S/kern/kern_descrip.c
	${COMPILE_C}

kern_exec.o: $S/kern/kern_exec.c
	${COMPILE_C}

kern_exit.o: $S/kern/kern_exit.c
	${COMPILE_C}

kern_fork.o: $S/kern/kern_fork.c
	${COMPILE_C}

kern_mman.o: $S/kern/kern_mman.c
	${COMPILE_C}

kern_proc.o: $S/kern/kern_proc.c
	${COMPILE_C}

kern_prot.o: $S/kern/kern_prot.c
	${COMPILE_C}

kern_prot2.o: $S/kern/kern_prot2.c
	${COMPILE_C}

kern_resource.o: $S/kern/kern_resource.c
	${COMPILE_C}

kern_sig.o: $S/kern/kern_sig.c
	${COMPILE_C}

kern_sig2.o: $S/kern/kern_sig2.c
	${COMPILE_C}

kern_subr.o: $S/kern/kern_subr.c
	${COMPILE_C}

kern_synch.o: $S/kern/kern_synch.c
	${COMPILE_C}

kern_sysctl.o: $S/kern/kern_sysctl.c
	${COMPILE_C}

kern_time.o: $S/kern/kern_time.c
	${COMPILE_C}

subr_prf.o: $S/kern/subr_prf.c
	${COMPILE_C}

subr_rmap.o: $S/kern/subr_rmap.c
	${COMPILE_C}

sys_generic.o: $S/kern/sys_generic.c
	${COMPILE_C}

sys_inode.o: $S/kern/sys_inode.c
	${COMPILE_C}

sys_pipe.o: $S/kern/sys_pipe.c
	${COMPILE_C}

sys_process.o: $S/kern/sys_process.c
	${COMPILE_C}

syscalls.o: $S/kern/syscalls.c
	${COMPILE_C}

tty.o: $S/kern/tty.c
	${COMPILE_C}

tty_subr.o: $S/kern/tty_subr.c
	${COMPILE_C}

tty_tty.o: $S/kern/tty_tty.c
	${COMPILE_C}

ufs_alloc.o: $S/kern/ufs_alloc.c
	${COMPILE_C}

ufs_bio.o: $S/kern/ufs_bio.c
	${COMPILE_C}

ufs_bmap.o: $S/kern/ufs_bmap.c
	${COMPILE_C}

ufs_dsort.o: $S/kern/ufs_dsort.c
	${COMPILE_C}

ufs_fio.o: $S/kern/ufs_fio.c
	${COMPILE_C}

ufs_inode.o: $S/kern/ufs_inode.c
	${COMPILE_C}

ufs_mount.o: $S/kern/ufs_mount.c
	${COMPILE_C}

ufs_namei.o: $S/kern/ufs_namei.c
	${COMPILE_C}

ufs_subr.o: $S/kern/ufs_subr.c
	${COMPILE_C}

ufs_syscalls.o: $S/kern/ufs_syscalls.c
	${COMPILE_C}

ufs_syscalls2.o: $S/kern/ufs_syscalls2.c
	${COMPILE_C}

vfs_vnops.o: $S/kern/vfs_vnops.c
	${COMPILE_C}

vm_sched.o: $S/kern/vm_sched.c
	${COMPILE_C}

vm_swap.o: $S/kern/vm_swap.c
	${COMPILE_C}

vm_swp.o: $S/kern/vm_swp.c
	${COMPILE_C}

cons.o: $S/dev/cons.c
	${COMPILE_C}

mem.o: $S/dev/mem.c
	${COMPILE_C}

swap.o: $S/dev/swap.c
	${COMPILE_C}

clock.o: $S/arch/pic32/pic32/clock.c
	${COMPILE_C}

conf.o: $S/arch/pic32/pic32/conf.c
	${COMPILE_C}

exception.o: $S/arch/pic32/pic32/exception.c
	${COMPILE_C}

machdep.o: $S/arch/pic32/pic32/machdep.c
	${COMPILE_C}

sig_machdep.o: $S/arch/pic32/pic32/sig_machdep.c
	${COMPILE_C}

sysctl.o: $S/arch/pic32/pic32/sysctl.c
	${COMPILE_C}

usb_device.o: $S/arch/pic32/hal/usb_device.c
	${COMPILE_C}

usb_function_cdc.o: $S/arch/pic32/hal/usb_function_cdc.c
	${COMPILE_C}

adc.o: $S/arch/pic32/dev/adc.c
	${COMPILE_C}

gpanel-ili9341.o: $S/arch/pic32/dev/gpanel-ili9341.c
	${COMPILE_C}

gpanel-nt35702.o: $S/arch/pic32/dev/gpanel-nt35702.c
	${COMPILE_C}

gpanel-st7781.o: $S/arch/pic32/dev/gpanel-st7781.c
	${COMPILE_C}

gpanel.o: $S/arch/pic32/dev/gpanel.c
	${COMPILE_C}

gpio.o: $S/arch/pic32/dev/gpio.c
	${COMPILE_C}

pwm.o: $S/arch/pic32/dev/pwm.c
	${COMPILE_C}

sd.o: $S/arch/pic32/dev/sd.c
	${COMPILE_C}

spi.o: $S/arch/pic32/dev/spi.c
	${COMPILE_C}

spi_bus.o: $S/arch/pic32/dev/spi_bus.c
	${COMPILE_C}

usb_uart.o: $S/arch/pic32/dev/usb_uart.c
	${COMPILE_C}


-include .deps/*.dep
