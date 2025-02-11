# DiscoBSD/stm32 - 2.11BSD-based OS for Arm Cortex-M4 MCUs

## Currently supported hardware

 * [WeAct Studio STM32F405RGT6 Core board][1]
 * [STMicroelectronics NUCLEO-F411RE][2]
 * [STMicroelectronics STM32F412G-DISCO][3]
 * [WeAct Studio STM32F412RET6 Core board][1]
 * [STMicroelectronics STM32F469I-DISCO][4]
 * [STMicroelectronics STM32F4DISCOVERY][5]

[1]: https://github.com/WeActStudio/WeActStudio.STM32F4_64Pin_CoreBoard
[2]: https://www.st.com/en/evaluation-tools/nucleo-f411re.html
[3]: https://www.st.com/en/evaluation-tools/32f412gdiscovery.html
[4]: https://www.st.com/en/evaluation-tools/32f469idiscovery.html
[5]: https://www.st.com/en/evaluation-tools/stm32f4discovery.html

## DiscoBSD/stm32 Up and Running

The file system image `sdcard.img` needs to be imaged onto an SD card.

On Windows host systems use a disk imaging utility such as [Rufus][6].

On Unix-like host systems with `dd` run:
  ```sh
    $ dd bs=32k if=sdcard.img of=/path/to/SD/card
  ```

The target `installfs` can be used to image the SD card on Unix-like systems.
Provide the path to the SD card via the command line:
  ```sh
    $ SDCARD=/path/to/SD/card make installfs
  ```

The board-specific kernel `unix` must be loaded into the MCU's flash memory.
Formats are ELF `unix.elf`, binary `unix.bin`, and Intel HEX `unix.hex`.

On Windows host systems use [STM32CubeProgrammer][7] for flash programming.

On Unix-like host systems use `st-flash` from the [stlink-org project][8] to
load the binary-formatted kernel `unix.bin` into flash memory at 0x08000000.
  ```sh
    $ st-flash --reset write unix.bin 0x08000000
  ```

[6]: https://github.com/pbatard/rufus
[7]: https://www.st.com/en/development-tools/stm32cubeprog.html
[8]: https://github.com/stlink-org/stlink

## Logging in to DiscoBSD

First, connect to the development board via the serial port.
The STM32 Disco and Nucleo boards offer a USB VCP serial port.
  ```sh
    $ cu -l /dev/cuaU0 -s 115200
  ```
Other systems may use serial port utilities such as `screen`, `minicom`,
`putty`, or `teraterm`.

Log in to DiscoBSD with user `root` and a blank password.

Shutdown DiscoBSD with:
  ```sh
    $ shutdown -h now
  ```
`halt` and `reboot` also bring down the system.

## Building the DiscoBSD/stm32 kernel and operating system on a Unix-like host
  ```sh
    $ make distribution
  ```

The kernel (for the f412gdisco board) can be built independently by:
  ```sh
    $ cd sys/stm32/f412gdisco
    $ make
  ```
Note: Building the kernel requires the `tools/kconfig/kconfig` config utility.

## Debugging DiscoBSD/stm32 on a development board via OpenOCD and GDB
  ```sh
    $ BOARD=f412gdisco make ocd
  ```

In a separate terminal, run `gdb` with:
  ```sh
    $ BOARD=f412gdisco make gdb-ocd
  ```

Running `make help` lists targets to debug DiscoBSD/stm32 with OpenOCD and GDB.
