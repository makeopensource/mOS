# Using GDB

To run gdb with qemu, simple run `qemu-gdb`! An instance of qemu will run in the
background while gdb is running in your current terminal.

For a refresher on gdb, see the following resources:

- [UMich GDB Tutorial](https://web.eecs.umich.edu/~sugih/pointers/summary.html)
- [RedHat: The GDB developer's GNU Debugger tutorial](https://developers.redhat.com/blog/2021/04/30/the-gdb-developers-gnu-debugger-tutorial-part-1-getting-started-with-the-debugger)

## Debugging the Bootloader

To debug the instructions in the bootloader, start by running `qemu-gdb-boot`. A
few things happen when you run this. First, we load a few default settings and
macros from `gdb_init_real_mode.txt` that allow us to better debug in
16-bit "real" mode. Using the `-ex` flag, we pass gdb a few commands. We connect
to qemu, then set a breakpoint at 0x7c00, which is the address where the bootloader 
code is located in memory.

Once you run `qemu-gdb-boot`, gdb will be pointing to the start of 0x7c00, where
you can then use gdb instruction commands or the macros provided by `gdb_init_real_mode.txt`
to debug the bootloader. Because the bootloader is written in assembly, you have
to use the assembly versions of gdb commands when debugging. See 
[here](https://www.cs.swarthmore.edu/~newhall/cs31/resources/ia32_gdb.php) for
more details.

See [Remote debugging of real mode code with gdb](https://ternet.fr/gdb_real_mode.html) 
for more information on the macro commands provided. Some useful ones include 
`step_until_int` and `step_until_ret`.

## Debugging the OS

To debug various parts of the OS, start by running `qemu-gdb`. When you run
this command, gdb connects to qemu and jumps to os_main, the first line of C
code in mOS.

Unlike real mode, 32-bit "protected" mode is a little easier to debug. You can
use regular gdb commands to debug as expected.
