# OpenBSD Development Environment

Note: These instructions are current as of OpenBSD 7.6.

This is the directory that maintains the development environment on OpenBSD.
It consists of a binary flash downloader, a circuit board emulator, and
a compiler, an assembler, a C library, and a source-level debugger,
as well as an on-chip debugger.

## Setting up the Development Environment for Arm target

All components of the development environment are available from the
OpenBSD base system, the OpenBSD package system, or as a user-compiled
OpenBSD port in the `mystuff` directory subtree.

Information about the OpenBSD Package System can be found at:
https://www.openbsd.org/faq/faq15.html

Packages need no setup.

These required utilities can be installed as Packages.
  ```sh
    $ pkg_add bison elftoolchain gettext-runtime
    $ pkg_add git groff openocd sudo unzip zip
  ```

The remaining required utilities must be built as Ports.

Information about the OpenBSD Ports System can be found at:
https://www.openbsd.org/faq/ports/ports.html

1. Fetch and Install the Ports Collection.
  ```sh
    $ cd /tmp
    $ ftp https://cdn.openbsd.org/pub/OpenBSD/$(uname -r)/ports.tar.gz
    $ cd /usr
    $ tar xzvf /tmp/ports.tar.gz
  ```

2. Add the Ports from this repo to the `/usr/ports/mystuff` directory.
  ```sh
    $ cd {where you cloned this repo}/tools/openbsd/ports
    $ cp -R mystuff /usr/ports/
  ```

### Binary Flash Downloader, Port: STLink V1.8.0 (Optional)
```sh
  $ cd /usr/ports/mystuff/devel/stlink
  $ sudo make
  $ sudo make install
```

### Circuit Board Emulator, Port: QEMU (Cortex-M) V2.8.0-6-20190517 (Optional)
```sh
  $ cd /usr/ports/mystuff/emulators/qemu-arm
  $ sudo make
  $ sudo make install
```

### Meta-package Toolchain: arm-none-eabi-{binutils,gcc,newlib,gdb}

#### Assembler, Package: Binutils V2.40, Configured for arm-none-eabi
```sh
  $ sudo pkg_add arm-none-eabi-binutils
```

#### Compiler, Port: GCC, V12.2.0, Configured for arm-none-eabi rmprofile
```sh
  $ cd /usr/ports/mystuff/devel/arm-none-eabi/gcc
  $ sudo make
  $ sudo make install
```

#### C Library, Port: Newlib V2.2.0-1, Configured for arm-none-eabi (Optional)
```sh
  $ cd /usr/ports/mystuff/devel/arm-none-eabi/newlib
  $ sudo make
  $ sudo make install
```

#### Debugger, Port: GNU Debugger V12.1, Configured for arm-none-eabi
```sh
  $ cd /usr/ports/mystuff/devel/arm-none-eabi/gdb
  $ sudo make
  $ sudo make install
```

### On-Chip Debugger, Package: OpenOCD V0.11.0
```sh
  $ sudo pkg_add openocd
```

Note: The whole meta-package can be compiled and installed by one command:
```sh
  $ cd /usr/ports/mystuff/devel/arm-none-eabi
  $ sudo make
  $ sudo make install
```

## Setting up the Development Environment for MIPS target

Note: These steps assume that all previous steps have been completed.

### Meta-package Toolchain: mips-elf-{binutils,gcc}

#### Assembler, Port: Binutils V2.40, Configured for mips-elf
```sh
  $ cd /usr/ports/mystuff/devel/mips-elf/binutils
  $ sudo make
  $ sudo make install
```

#### Compiler, Port: GCC V12.2.0, Configured for mips-elf
```sh
  $ cd /usr/ports/mystuff/devel/mips-elf/gcc
  $ sudo make
  $ sudo make install
```

Note: The whole meta-package can be compiled and installed by one command:
```sh
  $ cd /usr/ports/mystuff/devel/mips-elf
  $ sudo make
  $ sudo make install
```

### ELF Compilation Tools, Package: ELF Toolchain V0.7.1
```sh
  $ sudo pkg_add elftoolchain
```

### Internationalization Library, Package: GetText V0.22.5
```sh
  $ sudo pkg_add gettext-runtime
```
