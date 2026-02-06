# OpenBSD Development Environment

Note: These instructions are current as of OpenBSD 7.6.

This is the directory that maintains the development environment on OpenBSD.

Required programs to build a complete DiscoBSD distribution are:
  - Version control:        git
  - Native compiler:        clang or gcc
  - Utilities:              awk, bison, bmake, byacc, flex, gettext, groff
  - Utilities:              mandoc, pkg-config, sed, sudo, unzip, zip
  - Libraries:              libbsd, libelf, libfuse
  - Cross compiler (ARM):   binutils and gcc arm-none-eabi rmprofile
  - Cross debugger (ARM):   gdb arm-none-eabi or gdb multiarch
  - Firmware loader (ARM):  dfu-util, openocd, stlink
  - Cross compiler (MIPS):  binutils and gcc mips-elf
  - Cross debugger (MIPS):  gdb mips-elf or gdb multiarch
  - Firmware loader (MIPS): pic32prog

## Setting up the Development Environment for Arm target

All components of the development environment are available from the
OpenBSD base system, the OpenBSD package system, or as a user-compiled
OpenBSD port in the `mystuff` directory subtree.

Information about the OpenBSD Package System can be found at:
https://www.openbsd.org/faq/faq15.html

Packages need no setup.

These required utilities can be installed as Packages:
  ```sh
    $ pkg_add bison dfu-util elftoolchain gettext-runtime
    $ pkg_add git groff openocd sudo unzip zip
  ```

The remaining required utilities must be built as Ports.

Information about the OpenBSD Ports System can be found at:
https://www.openbsd.org/faq/ports/ports.html

1. Fetch and Install the Ports Collection:
  ```sh
    $ cd /tmp
    $ ftp https://cdn.openbsd.org/pub/OpenBSD/$(uname -r)/ports.tar.gz
    $ cd /usr
    $ tar xzvf /tmp/ports.tar.gz
  ```

2. Add the Ports from this repo to the `/usr/ports/mystuff` directory:
  ```sh
    $ cd {where this repo is cloned}/tools/openbsd/ports
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

### Meta-package Toolchain: arm-none-eabi-{binutils,gcc,gdb}

#### Assembler, Package: Binutils V2.40, Configured for arm-none-eabi
```sh
  $ sudo pkg_add arm-none-eabi-binutils
```
Note: __DO NOT__ install the default OpenBSD `arm-none-eabi-gcc` Package.
  It does not enable the `rmprofile` multilib, missing critical library files.
  Instead, compile the custom-configured GCC port as outlined below.

#### Compiler, Port: GCC, V12.2.0, Configured for arm-none-eabi rmprofile
```sh
  $ cd /usr/ports/mystuff/devel/arm-none-eabi/gcc
  $ sudo make
  $ sudo make install
```

#### Debugger, Port: GNU Debugger V12.1, Configured for arm-none-eabi
```sh
  $ cd /usr/ports/mystuff/devel/arm-none-eabi/gdb
  $ sudo make
  $ sudo make install
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
