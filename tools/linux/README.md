# Linux Development Environment

Note: These instructions are current as of Ubuntu 24.04.

This is the directory that maintains the development environment on Linux.

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

All components of the development environment are available as Packages.

Information about APT-based Package Management on Linux can be found at:
https://wiki.debian.org/PackageManagement

These required utilities can be installed as Packages:
  ```sh
    $ apt install build-essential {binutils,gcc}-arm-none-eabi git sudo
    $ apt install gdb-multiarch dfu-util openocd stlink-tools pkg-config
    $ apt install bison bmake byacc flex gawk gettext-base gsed mandoc
    $ apt install libbsd-dev libelf-dev libfuse-dev groff-base unzip zip
  ```

Note: So that `make gdb-ocd` works, create the following symlink on the host
      system if `gdb-multiarch` is installed instead of `gdb-arm-none-eabi`:
  ```sh
    $ ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb
  ```

## Setting up the Development Environment for MIPS target

Note: These steps assume that all previous steps have been completed.

### Toolchain: mips-elf-{binutils,gcc,gdb} GCC V4.8.1, Binutils V2.23.2

Build the MIPS toolchain from source, as described on page:
https://web.archive.org/web/20200126100825/http://retrobsd.org/wiki/doku.php/doc/toolchain-mips

### Binary Flash Downloader: pic32prog V2.1.57

Available from:
https://github.com/majenkotech/pic32prog-autotools/archive/refs/tags/2.1.57.zip
