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

## General behavior
boot and immediately drop to a prompt.
programs can be executed from the prompt.
send signals by keyboard to do ✨stuff✨.


## Resources
* https://www.qemu.org/
* https://wiki.osdev.org/Main_Page

* https://wiki.osdev.org/Linker_Scripts
