# DiscoBSD/pic32 - 2.11BSD-based OS for PIC32MX7 MIPS MCUs

## Currently supported hardware

 * [Fubarino SD][1] board.
 * [Olimex Duinomite][2], [Duinomite-Mini][3], [Duinomite-Mega][4] and
   [Duinomite-eMega][5] boards.
 * [Olimex Pinguino-Micro][6] board with PIC32MX795F512H microcontroller.
 * [Maximite][7] and [Colour Maximite][8] computers.
 * [Majenko SDXL][9] board.
 * [4D Systems Picadillo-35T][10] board.
 * [MikroElektronika MultiMedia Board][11] for PIC32MX7.
 * [chipKIT Max32][12] board with SD card shield.
 * [chipKIT WF32][13] board with 2.4" LCD TFT display shield.
 * [Sparkfun UBW32][14] board with SD card slot.
 * [Microchip Explorer 16][15] board,
   with PIC32 CAN-USB plug-in module and SD & MMC pictail.
 * [Microchip PIC32 USB][16] or [Ethernet Starter Kit][17],
   with I/O Expansion board and SD & MMC pictail.
 * [Pontech Quick240][18] Quick Universal Industrial Control Kard
   system based on the chipKIT platform.

[1]: https://www.fubarino.org/sd/
[2]: https://www.olimex.com/Products/Duino/Duinomite/DUINOMITE/
[3]: https://www.olimex.com/Products/Duino/Duinomite/DUINOMITE-MINI/
[4]: https://www.olimex.com/Products/Duino/Duinomite/DUINOMITE-MEGA/
[5]: https://www.olimex.com/Products/Duino/Duinomite/DUINOMITE-eMEGA/
[6]: https://www.olimex.com/Products/Duino/PIC32/PIC32-RETROBSD/
[7]: https://geoffg.net/MonoMaximite.html
[8]: https://geoffg.net/OriginalColourMaximite.html
[9]: https://wiki.kewl.org/boards:sdxl
[10]: https://resources.4dsystems.com.au/datasheets/legacy/Picadillo-35T_datasheet_R_1_5.pdf
[11]: https://web.archive.org/web/20160815090501/http://www.mikroe.com/multimedia/pic32mx7/
[12]: https://chipkit.net/wiki/index.php?title=chipKIT_Max32
[13]: https://chipkit.net/wiki/index.php?title=chipKIT_WF32
[14]: https://www.schmalzhaus.com/UBW32/
[15]: https://www.microchip.com/en-us/development-tool/dm240001
[16]: https://www.microchip.com/en-us/development-tool/dm320003-2
[17]: https://www.microchip.com/en-us/development-tool/dm320004
[18]: https://quick240.com

## Build

A few packages are required to compile everything from source.
Under Ubuntu installation can be done by the commands:

```sh
$ apt install bison byacc flex git groff-base libbsd-dev
$ apt install libelf-dev libfuse-dev sudo unzip zip
```
If a mips-elf-gcc compiler package is not available from the host distribution,
then the compiler toolchain must be built following [these instructions][18].

[18]: https://web.archive.org/web/20200126100825/http://retrobsd.org/wiki/doku.php/doc/toolchain-mips

The desired filesystem size and swap area size can be changed, as required.
Default is:
```Makefile
FS_MBYTES   = 200
U_MBYTES    = 200
SWAP_MBYTES = 2
```
To compile the kernel and build a filesystem image, run:

```sh
$ make MACHINE=pic32 MACHINE_ARCH=mips distribution
```

A resulting root filesystem image is in the file `sdcard.img`.
Kernel files are named `unix.hex` and are in target board subdirectories.

### Filesystem image

The file system image `sdcard.img` needs to be imaged onto an SD card.

On Windows host systems use a disk imaging utility such as [Rufus][19].

On Unix-like host systems with `dd` run:
```sh
$ sudo dd if=sdcard.img of=/path/to/SD/card
```

[19]: https://github.com/pbatard/rufus

### Install kernel

The kernel image must be written to the PIC32 flash memory.
The specific procedure depends on the target development board.

#### PIC32-RETROBSD board:
Use the [pic32prog][20] utility and a USB cable to install the kernel:

```sh
$ pic32prog sys/pic32/pinguino-micro/unix.hex
```

#### Max32 board:
Use the [pic32prog][20] utility and a USB cable to install the kernel:

```sh
$ pic32prog -d /dev/ttyUSB0 sys/pic32/max32/unix.hex
```

#### UBW32 board:
Use the [pic32prog][20] utility and a USB cable to install the kernel:

```sh
$ pic32prog sys/pic32/ubw32/unix.hex
```

#### Maximite:
Use the bootload program for Windows, download links are available here:
https://geoffg.net/MonoMaximite.html#Downloads

#### Explorer 16 board:
There is an auxiliary PIC18 chip on the Explorer 16 board, which can be
used as a built-in programmer device.  A PICkit 2 adapter is needed to
install the required firmware, as described in [this article][21] in the
section "Hack #2: Lose the PICkit 2, Save $35".
This should be done only once.

Then, use the [pic32prog][20] utility and a USB cable to install the kernel:

``` sh
$ pic32prog sys/pic32/explorer16/unix.hex
```

#### PIC32 Starter Kit:
Use the PICkit 2 adapter and software to install a boot loader from the file
[starter-kit/bootloader.hex][22] in the [pic32-usb-bootloader][23] repository.
This should be done only once.

Then, use the [pic32prog][20] utility and a USB cable to install the kernel:

```sh
$ pic32prog sys/pic32/starter-kit/unix.hex
```

[20]: https://github.com/majenkotech/pic32prog-autotools/archive/refs/tags/2.1.57.zip
[21]: https://web.archive.org/web/20160506100841/http://www.paintyourdragon.com/?p=51
[22]: https://github.com/sergev/pic32-usb-bootloader/blob/master/starter-kit/bootloader.hex
[23]: https://github.com/sergev/pic32-usb-bootloader

## Simulator

Use the VirtualMIPS MIPS32 simulator to develop and debug DiscoBSD/pic32
without the need for a hardware development board.
By default, the simulator is configured to imitate a Max32 board.
To build it:

```sh
$ cd tools/virtualmips
$ make
```

Run it:

```sh
$ ./pic32
```

Configuration of the simulated board is stored in the file `pic32_max32.conf`.
