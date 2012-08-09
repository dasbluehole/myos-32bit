include x86/Makefile
include kernel/Makefile
include lib/Makefile
include driver/Makefile
#include net/Makefile
#include shell/Makefile

INCLUDE := -I./include 
CFLAGS  := $(INCLUDE) -Wall -Wextra -nostdlib -nostdinc -nostartfiles -nodefaultlibs -fno-builtin -c -g 
CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions -fpic 

all: clean $(OBJS)
	@echo " [LD]        kernel"
	@ld -Map Tiny32.map -T link.ld $(OBJS) -o ./bin/kernel.elf

.c.o:
	@echo " [GCC]		"$@
	@gcc -o $@ $? $(CFLAGS)

.cpp.o:
	@echo " [G++]		"$@
	@gcc -o $@ $? $(CXXFLAGS)

.s.o:
	@echo " [ASM]		"$@
	@nasm -g -felf -o $@ $?

clean:
	@echo "Cleaning object files"
	@rm -f $(OBJS)
	@echo "Removing Kernel"
	@rm -f ./bin/*.elf
	@echo "Removing LiveCD if any"
	@rm -f *.iso
	@echo removing iso folder
	@rm -rf iso

iso2: all
	mkdir -p iso/boot/grub
	cp ./bin/kernel.elf ./iso/boot
	cp grub.cfg ./iso/boot/grub
	grub2-mkrescue -o Tiny32.iso iso

floppy: all
		
	@echo "Mounting floppy image "
	@sudo mount /tmp/nanoos.img /mnt -o loop
	@echo "Copying kernle image "
	@sudo cp -ip ./bin/nanos.elf /mnt/boot/nanos.elf
	@sync
	@echo "Unmounting Floppy image"
	@sudo umount /mnt
	@echo DATE

