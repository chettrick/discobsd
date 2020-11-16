PROG=		003_cmsis

# Program sources.
SRCS=		main.c
SRCS+=		startup_stm32f407xx.s
SRCS+=		stm32f4xx_it.c
SRCS+=		system_stm32f4xx.c

# Library sources.
SRCS+=		STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_utils.c


TOOLCHAIN=	arm-none-eabi
AR=		${TOOLCHAIN}-ar
CC=		${TOOLCHAIN}-gcc
OBJCOPY=	${TOOLCHAIN}-objcopy
SIZE=		${TOOLCHAIN}-size

# Compiler flags.
CFLAGS=		-g -O2 -Wall
CFLAGS+=	-TSTM32F407XG.ld
CFLAGS+=	-mcpu=cortex-m4
CFLAGS+=	-mlittle-endian -mthumb
CFLAGS+=	-mno-thumb-interwork
CFLAGS+=	-mfloat-abi=soft -mfpu=fpv4-sp-d16
CFLAGS+=	-ffreestanding
CFLAGS+=	--specs=nosys.specs

# Preprocessor flags.
CPPFLAGS+=	-DSTM32F407xx
CPPFLAGS+=	-I.
CPPFLAGS+=	-ISTM32F4xx_HAL_Driver/Inc

CLEANFILES+=	${PROG}.elf ${PROG}.hex ${PROG}.bin *.o


all: prog

prog: ${PROG}.elf

${PROG}.elf: ${SRCS}
	${CC} ${CFLAGS} ${CPPFLAGS} ${SRCS} ${LDFLAGS} -o $@
	${OBJCOPY} -O ihex   ${PROG}.elf ${PROG}.hex
	${OBJCOPY} -O binary ${PROG}.elf ${PROG}.bin
	${SIZE} ${PROG}.elf

clean:
	rm -f ${CLEANFILES}
