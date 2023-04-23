# GCC mips-elf toolchain on OpenBSD, FreeBSD, and Linux
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Build the toolchain with the Ports Collection for OpenBSD,
# as described in the file: tools/openbsd/README.md
#
# Build the toolchain from sources for FreeBSD and Linux,
# as described on page: http://retrobsd.org/wiki/doku.php/doc/toolchain-mips

_HOST_OSNAME!=	uname -s

MIPS_GCC_PREFIX!= if [ x"${_HOST_OSNAME}" = x"OpenBSD" ] ; then \
			echo "/usr/local/bin/mips-elf-" ; \
		elif [ x"${_HOST_OSNAME}" = x"FreeBSD" ] ; then \
			echo "/usr/local/mips-elf/bin/mips-elf-" ; \
		elif [ x"${_HOST_OSNAME}" = x"Linux" ] ; then \
			echo "/usr/local/mips-gcc-4.8.1/bin/mips-elf-" ; \
		else \
			echo "/does/not/exist-" ; \
		fi
