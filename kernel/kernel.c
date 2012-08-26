/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  Our Kernel Proper                                      |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#include "low-io.h"
#include "mboot.h"
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "kbd.h"
#include "kheap.h"
#include "pci.h"
#include "ide.h"
#include "../driver/rtl/myrtl.h"

unsigned int kend;
extern unsigned start;
unsigned int total_mem;

int kmain(int magic,multibootInfo *mbootinfo)
{
   	
	init_VGA();
	
	SetBackColour(BRIGHTWHITE);
	SetTextColour(BRIGHTBLUE);
	clear();
	kprintf("\t****************Hello Tiny OS-32****************\n");
	kprintf("\t*****************Keep Patience******************\n");
	kprintf("\t================================================\n");
	if(magic != 0x2BADB002)
	{
		kprintf("BAD MULTIBOOT MAGIC!!! %010x Exiting",magic);
		while(1);
	}
	kend = mboot.kernel_end;
	show_memory_map(mbootinfo);
	total_mem = get_available_memory(mbootinfo);
	kprintf("Memory available =  %08x\n",get_available_memory(mbootinfo) );
	show_elf_info(mbootinfo);
	kprintf("Starting Memory Manager(very basic: thanks Chris)\n");	
	init_heap();
	SetBackColour(BLACK);	
	SetBackColour(BLACK);	
	SetTextColour(BRIGHTWHITE);
	show_memory_map(mbootinfo);	
	kprintf("Setting up GDT\t");
	setup_GDT();
	kprintf("Done\n");
		
	kprintf("Setting up IDT\t");
	setup_IDT();
	kprintf("Done\n");
	kprintf("Setting up IRQs\t");
	irq_install();
	kprintf("Done\n");
	kprintf("\tInstalling Timer handler\t");
	install_timer();
	kprintf("Done\n");
	kprintf("\tInitializing Key-Board\t");
	install_kbd();
	kprintf("Done\n"); 
	
	// pci_bus scan, it will create a global pci_list which is a doubly linked list of pci devices found	
	pci_scan();
	SetTextColour(BRIGHTWHITE);
	//init_floppy();
	kprintf("TODO:scheduler,threads OR processes, Shell,Few applications ,etc\n");
	
	enable();
	//clear();
	//init_tasks();	
	init_ide();
// this block may be used latter
	unsigned int *bd;
	bd = (unsigned int *)get_boot_dev(mbootinfo);
	kprintf("%0x %0x %0x %0x\n",(*bd & 0x000000ff),(*bd & 0x0000ff00)>>8,(*bd & 0x00ff0000)>>16,(*bd & 0xff000000)>>24 );
	unsigned char ba[5]="";
	strcat(ba,get_boot_dev(mbootinfo));
	kprintf("%0x %0x %0x %0x\n",ba[0]&0xff,ba[1]&0xff,ba[2]&0xff,ba[3]&0xff);
	
	//r_buf=(unsigned char*)kmalloc(512);
	//memset(r_buf,0,512);
	//for(i=0;i<128;i++)
	//kprintf("%2x ",r_buf[i]);
	//read_sector(0x1f0,1,&r_buf[0]);
	//for(i=0;i<128;i++)
	//kprintf("%2x ",r_buf[i]);	
	//dump(r_buf,512);
	//do_page_fault= *ptr;
//	setVideoMode(12);
//	box(100,100,400,400);
//	init_windows();
//	errmsgbox("Test Error$","Hello World this is not enough$");
//	displaymessages(100,100,"Hello World$",4);
	detect_netdev();
	dump_irq_routines();
	while(1);	
	halt();		
}
