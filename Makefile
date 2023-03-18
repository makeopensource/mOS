CC=gcc
CFLAGS=-Wall -Werror -g3 -Os -Wl,--oformat=binary -no-pie -m32 -s -falign-functions=4 -ffreestanding -masm=intel -fno-asynchronous-unwind-tables -I./src/lib/
LFLAGS=-melf_i386 --build-id=none

ASM_BOOT_SECT_SOURCE=./src/boot/boot_sect.asm
ASM_OS_ENTRY_SOURCE=./src/boot/os_entry.asm

BOOT_OBJ := boot.o

OBJ_NAMES := src/os/main.o os_entry.o src/lib/video/VGA_text.o \
	src/os/hard/idt.o src/os/hard/except.o src/os/hard/pic.o

.PHONY: clean qemu

mOS.bin: $(OBJ_NAMES) $(BOOT_OBJ)
	ld $(LFLAGS) -T link.ld $(OBJ_NAMES) -o mOS.elf
	objcopy -O binary mOS.elf intermediate.bin
	cat boot.o intermediate.bin > mOS.bin 

boot.o: $(ASM_BOOT_SECT_SOURCE)
	nasm $^ -f bin -o $@ 

os_entry.o: $(ASM_OS_ENTRY_SOURCE)
	nasm $^ -f elf32 -o $@

%.o: %.c
	gcc -c $^ -o $@ $(CFLAGS)

%.o: %.asm
	nasm $^ -f elf32 -o $@

qemu: mOS.bin
	qemu-system-i386 -boot c -hda $^ -no-reboot -no-shutdown

clean:
	rm -f mOS
	rm -f *.o $(OBJ_NAMES) *.bin *.elf *.~ src/*~ src/boot/*~ docs/*~
