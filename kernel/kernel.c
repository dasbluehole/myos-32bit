#include "low-io.h"
#include "mboot.h"
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "kbd.h"
#include "kheap.h"
#include "pci.h"

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
	SetBackColour(BLACK);	
	SetTextColour(RED);
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
	
	show_memory_map(mbootinfo);
	total_mem = get_available_memory(mbootinfo);
	kprintf("Memory available =  %08x\n",get_available_memory(mbootinfo) );
	show_elf_info(mbootinfo);
	kprintf("Starting Memory Manager(very basic: thanks Chris)\n");	
	init_heap();
	SetBackColour(BLACK);
	SetTextColour(BRIGHTCYAN);
	// pci_bus scan, it will create a global pci_list which is a doubly linked list of pci devices found	
	pci_scan();
	SetTextColour(WHITE);
	//init_floppy();
	kprintf("TODO:scheduler,threads OR processes, Shell,Few applications ,etc\n");
	
	enable();
	//clear();
	init_tasks();	
	//detect_ide();
	
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

	while(1);	
	halt();		
}
