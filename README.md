# CSC490 - Porting the Unix Kernel

## Project Deliverables
The **Porting the Unix Kernel** project report is available [here](https://github.com/chettrick/CSC490/blob/master/project_outputs/Porting_the_Unix_Kernel-CSC490-Christopher_Hettrick.pdf).

A Zip archive of the code at the conclusion of the project is available [here](https://github.com/chettrick/CSC490/blob/master/project_outputs/discobsd.zip).

## Building the MIPS-based RetroBSD kernel and operating system on an OpenBSD host
 ```sh
    $ cd discobsd
    $ MACHINE=pic32 MACHINE_ARCH=mips gmake
  ```
  Note: The mips-elf gcc compiler sometimes segfaults when building the kernel.

  If this happens, just re-run the `gmake` step above until the build succeeds.

## Running the MIPS-based RetroBSD on the VirtualMIPS simulator
 ```sh
    $ cd discobsd/tools/virtualmips
    $ ./pic32
  ```
Log in to RetroBSD with user `root` and a blank password.

Shutdown RetroBSD with:
 ```sh
    # shutdown -h now
  ```

A screenshot of the booting process is below:

![Image of RetroBSD booting on VirtualMIPS simulator](https://github.com/chettrick/CSC490/blob/master/report/RetroBSD_boot_on_VirtualMIPS.png)


## Building the Arm-based DiscoBSD kernel and operating system on an OpenBSD host
 ```sh
    $ cd discobsd
    $ gmake
  ```
Note: The userland does not fully build. You can force building everything with:
   ```sh
    $ cd discobsd
    $ gmake -k
  ```

The kernel can be built independently by:
   ```sh
    $ cd discobsd/sys/stm32/f4discovery
    $ gmake
  ```
Note: The kernel requires the `discobsd/tools/kconfig/kconfig` program for building.

## Running the Arm-based DiscoBSD on the QEMU emulator and with gdb
 ```sh
    $ cd discobsd
    $ gmake qemu
  ```

In a separate terminal, run `gdb` with:
 ```sh
    $ cd discobsd
    $ gmake gdb-qemu
  ```

The command `gmake help` will give a list of targets devoted to running DiscoBSD, either on QEMU or with OpenOCD.

Note: Currently, the DiscoBSD kernel only blinks an LED.

A screenshot of the booting process is below:

![Image of DiscoBSD booting on QEMU Emulator](https://github.com/chettrick/CSC490/blob/master/report/DiscoBSD_boot_on_QEMU.png)
