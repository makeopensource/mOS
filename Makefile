CC=gcc
CFLAGS=-Wall -Werror -Os -Wl,--oformat=binary -no-pie -m32 -s -falign-functions=4 -ffreestanding -masm=intel -fno-asynchronous-unwind-tables
LFLAGS=-melf_i386 --build-id=none 

ASM_BOOT_SECT_SOURCE=./src/boot/boot_sect.asm
ASM_OS_ENTRY_SOURCE=./src/boot/os_entry.asm

C_MAIN_SOURCE=./src/main.c

OBJ_NAMES=boot.o main.o os_entry.o
LINK_OBJ_NAMES=main.o os_entry.o

.PHONY: clean qemu

mOS.bin: $(OBJ_NAMES)
	ld $(LFLAGS) -T link.ld $(LINK_OBJ_NAMES) -o mOS.elf
	objcopy -O binary mOS.elf intermediate.bin
	cat boot.o intermediate.bin > mOS.bin 

main.o: $(C_MAIN_SOURCE)
	gcc -c $^ -o $@ $(CFLAGS)

boot.o: $(ASM_BOOT_SECT_SOURCE)
	nasm $^ -f bin -o $@ 

os_entry.o: $(ASM_OS_ENTRY_SOURCE)
	nasm $^ -f elf32 -o $@ 

qemu: mOS.bin
	qemu-system-i386 -boot c -hda $^

clean:
	rm -f mOS
	rm -f *.o *.bin *.elf *.~ src/*~ src/boot/*~ docs/*~
