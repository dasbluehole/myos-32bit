#!/bin/bash
echo "Tiny OS Compilation script"
echo " In future it will be replaced by a Makefile"
echo "============================================"
echo 
echo
echo "Compiling C files"
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c kernel.c -o kernel.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c vga1.c -o vga.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c string.c -o string.o
#gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c vsprintf.c -o vsprintf.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c gdt.c -o gdt.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c mboot.c -o mboot.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c idt.c -o idt.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c irq.c -o irq.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c timer.c -o timer.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c kbd.c -o kbd.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c phys_mem.c -o phys_mem.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c paging.c -o paging.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c kmalloc.c -o kmalloc.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c dma.c -o dma.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c fdc.c -o fdc.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c gvga.c -o gvga.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c char.c -o char.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c windows.c -o windows.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c label.c -o label.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c pci.c -o pci.o
gcc  -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs -c ide.c -o ide.o

echo "assembling .asm files"
nasm -f elf loader.asm -o loader.o
#nasm -f elf low-io.asm -o low-io.o
nasm -f elf isr_wrap.asm -o isr_wrap.o
echo
echo "making the kernel by linking all"
#ld -T link.ld loader.o low-io.o vga.o string.o gdt.o vsprintf.o mboot.o idt.o isr_wrap.o irq.o timer.o kbd.o kernel.o -o kernel
ld -T link.ld loader.o vga.o string.o gdt.o mboot.o idt.o isr_wrap.o \
irq.o timer.o kbd.o phys_mem.o paging.o kmalloc.o gvga.o char.o windows.o \
label.o pci.o dma.o fdc.o ide.o kernel.o -o kernel
echo "done"
mkdir -p iso/boot/grub
cp ./kernel ./iso/boot
cp grub.cfg ./iso/boot/grub
grub2-mkrescue -o TinyOS32.iso iso
echo " copy this kernel to a grub floppy and boot from it"
rm -f *.o
rm -f *.*~
