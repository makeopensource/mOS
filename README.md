# MakeOpenSource Operating System
The major goal of this project is to recreate some of the functionality from a DOS-like operating system.

## Objectives (in no particular order)
* BIOS boot
* Basic memory initialization
* PS2 keyboard support (scan code conversion)
* VGA 40/80 column text mode console
* Commands (programs)
    * cd
    * pwd
    * ls
    * mkdir
    * rm
    * cp
    * clear (C-L)
    * man
    * vi
    * ed

## Contributing
For contributing _fork_ this repository and then make your changes in the form of Pull Requests to the main repository

## Building and running
### Dependencies
 * make
 * qemu (specifically `qemu-system-x86`) on debian based systems you can install this with the apt package manager
 * gcc
 * nasm

### Building
 * just run `make` in the root directory of the project

### Running
 * run `make qemu` in the root directory of the project

## General behavior
boot and immediately drop to a prompt.
programs can be executed from the prompt.
send signals by keyboard to do ✨stuff✨.


## Resources
* https://www.qemu.org/
* https://wiki.osdev.org/Main_Page

* https://wiki.osdev.org/Linker_Scripts


## FAQ
* If you get an error like 
```
WARNING: Image format was not specified for 'mOS.bin' and probing guessed raw.
	Automatically detecting the format is dangerous for raw images, write operations on block 0 will be restricted
	Specify the 'raw' format explicitly to remove the restrictions.

```
You probably don't have the right qemu, see the second dependency.
