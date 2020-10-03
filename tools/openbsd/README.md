# OpenBSD Development Environment

This is the directory that maintains the development environment on OpenBSD
for this Directed Study.
It consists of a binary flash downloader, a circuit board emulator, and
a compiler, an assembler. a C library, and a source-level debugger,
as well as an on-chip debugger.

## Setting up the Development Environment

All components of the development environment are available as an OpenBSD
package, or as a user-compiled OpenBSD port in the `mystuff` directory subtree.

Information about the Package System can be found at:
https://www.openbsd.org/faq/faq15.html

Packages need no setup.

Information about the Ports System can be found at: 
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

### Binary Flash Downloader, Port: STLink V1.1.0
```sh
  $ cd /usr/ports/mystuff/devel/stlink
  $ sudo make
  $ sudo make install
```

### Circuit Board Emulator, Port: GNU MCU (Arm) Eclipse QEMU V2.8.0-6-20190517
```sh
  $ cd /usr/ports/mystuff/emulators/qemu-arm
  $ sudo make
  $ sudo make install
```

### Meta-package Toolchain, Package: arm-none-eabi-{binutils,gcc,newlib,gdb}

#### Assembler: Binutils V2.27, Configured for arm-none-eabi
```sh
  $ sudo pkg_add arm-none-eabi-binutils
```

#### Compiler: GCC, Linaro Version V7.4-2019.02, Configured for bare-metal Arm
```sh
  $ sudo pkg_add arm-none-eabi-gcc-linaro
```

#### C Library: Newlib V2.2.0-1, Configured for arm-none-eabi
```sh
  $ sudo pkg_add arm-none-eabi-newlib
```

#### Source-Level Debugger: GNU Debugger V7.9.1, Configured for arm-none-eabi
```sh
  $ sudo pkg_add arm-none-eabi-gdb
```

Note that this version of gdb conflicts with the normal gdb package.
Only one version of gdb may be installed at any time.

### On-Chip Debugger, Package: OpenOCD V0.10.0
```sh
  $ sudo pkg_add openocd
```
