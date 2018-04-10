# MIN-OS #

= Minimalistic operating system for (my) educational purposes.

### Brief Info ###

* One micro-kernel
* Intended for x86 architecture
* Currently running only in Real mode
* Written mainly in C (GNU C99 standard), with assembly language for low-level operations

### Running ###
For running the operating system, You just need to:

* have a GRUB multi-boot loader
* be on x86 architecture
* have 32 kB of RAM

### Building ###
**Min-os might be fully build using these scripts:**

* `scripts/build.sh` or `./scripts/build_and_run.sh`


Using the above scripts, something like following will happen:

* Compilation of an assembly file for GRUB loading (`bootloaders/boot_grub.asm`)
* Compilation of all sources within the `source/src` folder (`kernel.c` in the first place) -- this is done by the `Makefile`
* Link step when the kernel (and other object fragments) are being linked using a custom linker/loader script
* Cleaning step
* Creating of ISO file intended for the GRUB bootloader


**Requirements for Building/Running**

* Unix system -- scripts are using utilities like `find`, `make`, etc.
* GCC compiler (cross-compiler)
* linker `ld` from GNU binutils
* NASM - the Net Wide Assembler (http://www.nasm.us), or GNU Assembler
* (Program `grub-mkrescue` for creating an ISO, see https://www.gnu.org/software/grub)
* (Virtual machine for running the OS using the GRUB -- for instance QEMU, see http://wiki.qemu.org)

### TODOs ###

* File System
* Real VGA driver
* Interrupt System Routines
* Memory manager (for dynamic allocation)
* Synchronization Primitives - Spinlocks, etc.


### Screenshot ###

![min-os1.png](https://bitbucket.org/repo/kAyX66/images/1379807843-min-os1.png)