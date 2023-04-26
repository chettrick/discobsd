# DiscoBSD/stm32 - 2.11BSD-based OS for Arm Cortex-M4 MCUs


## Currently supported hardware

 * NUCLEO-F411RE
 * STM32F412G-DISCO
 * STM32F469I-DISCO
 * STM32F4DISCOVERY


## DiscoBSD/stm32 Up and Running

The file system image `sdcard.img` needs to be imaged onto an SD card.
On host systems with `dd` run:
  ```sh
    $ dd bs=32k if=sdcard.img of=/path/to/SD/card
  ```

The target `installfs` can be used to image the SD card on Unix-like systems.
Provide the path to the SD card via the command line:
  ```sh
    $ SDCARD=/path/to/SD/card gmake installfs
  ```

The board-specific ELF-formatted kernel `unix` must be loaded into the MCU's flash memory.
Using OpenOCD or `st-flash` from the stlink-org project are two of the many methods.


## Logging in to DiscoBSD

First, connect to the development board via the serial port.
The STM32 Disco and Nucleo boards offer a USB VCP serial port.
  ```sh
    $ cu -l /dev/cuaU0 -s 115200
  ```
Other systems may use serial port utilities such as `screen`, `minicom`, `putty`, or `teraterm`.

Log in to DiscoBSD with user `root` and a blank password.

Shutdown DiscoBSD with:
  ```sh
    $ shutdown -h now
  ```
`halt` and `reboot` also bring down the system.


## Building the DiscoBSD/stm32 kernel and operating system on a Unix-like host
  ```sh
    $ gmake
  ```

The kernel (for the f412gdisco board) can be built independently by:
  ```sh
    $ cd sys/stm32/f412gdisco
    $ gmake
  ```
Note: The kernel requires the `tools/kconfig/kconfig` utility for kernel configuration.


## Debugging DiscoBSD/stm32 on a development board via OpenOCD and GDB
  ```sh
    $ BOARD=f412gdisco gmake ocd
  ```

In a separate terminal, run `gdb` with:
  ```sh
    $ BOARD=f412gdisco gmake gdb-ocd
  ```

The command `gmake help` gives a list of targets devoted to running DiscoBSD/stm32 with OpenOCD and GDB.
