# GCC arm-none-eabi toolchain on OpenBSD and Linux
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

_HOST_OSNAME!=	uname -s

ARM_GCC_PREFIX!=if [ x"${_HOST_OSNAME}" = x"OpenBSD" ] ; then \
			echo "/usr/local/bin/arm-none-eabi" ; \
		elif [ x"${_HOST_OSNAME}" = x"FreeBSD" ] ; then \
			echo "/usr/local/gcc-arm-embedded/bin/arm-none-eabi" ; \
		elif [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
			echo "/usr/bin/arm-none-eabi" ; \
		else \
			echo "/does/not/exist" ; \
		fi

_AS=		-gcc
_CC=		-gcc
_CPP=		-cpp
_LD=		-ld
_SIZE=		-size
_OBJCOPY=	-objcopy
_OBJDUMP=	-objdump

AS=		${ARM_GCC_PREFIX}${_AS}
CC=		${ARM_GCC_PREFIX}${_CC}
CPP=		${ARM_GCC_PREFIX}${_CPP}
LD=		${ARM_GCC_PREFIX}${_LD}
SIZE=		${ARM_GCC_PREFIX}${_SIZE}
OBJCOPY=	${ARM_GCC_PREFIX}${_OBJCOPY}
OBJDUMP=	${ARM_GCC_PREFIX}${_OBJDUMP}

LIBGCC=		`${CC} ${CMACHCPU} -print-libgcc-file-name`
