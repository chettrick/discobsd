# GCC mips-elf toolchain on OpenBSD, FreeBSD, and Linux
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Build the toolchain with the Ports Collection for OpenBSD,
# as described in the file: tools/openbsd/README.md
#
# Build the toolchain from sources for FreeBSD and Linux,
# as described on page: http://retrobsd.org/wiki/doku.php/doc/toolchain-mips

_HOST_OSNAME!=	uname -s

MIPS_GCC_PREFIX!= if [ x"${_HOST_OSNAME}" = x"OpenBSD" ] ; then \
			echo "/usr/local/bin/mips-elf" ; \
		elif [ x"${_HOST_OSNAME}" = x"FreeBSD" ] ; then \
			echo "/usr/local/mips-elf/bin/mips-elf" ; \
		elif [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
			echo "/usr/local/mips-gcc-4.8.1/bin/mips-elf" ; \
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

AS=		${MIPS_GCC_PREFIX}${_AS}
CC=		${MIPS_GCC_PREFIX}${_CC}
CPP=		${MIPS_GCC_PREFIX}${_CPP}
LD=		${MIPS_GCC_PREFIX}${_LD}
SIZE=		${MIPS_GCC_PREFIX}${_SIZE}
OBJCOPY=	${MIPS_GCC_PREFIX}${_OBJCOPY}
OBJDUMP=	${MIPS_GCC_PREFIX}${_OBJDUMP}
