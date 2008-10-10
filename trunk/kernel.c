#include "low-io.h"
#include "mboot.h"
#include "vga.h"
#include "kmalloc.h"
#include "windows.h"

extern unsigned end;
extern unsigned start;
extern unsigned int total_mem;
extern void probe_pci();
extern void pci_scan();
int main(multibootInfo *mbootinfo)
{
	//unsigned long mem_avail,mem_used,test_mem,tot_mem;
	unsigned int *ptr = (unsigned int*)0xA0000000;
   	unsigned int do_page_fault,i,*int_ptr;	
	init_VGA();
	
	SetBackColour(BRIGHTWHITE);
	SetTextColour(BRIGHTBLUE);
	clear();
	kprintf("\t****************Hello Tiny OS-32****************\n");
	kprintf("\t*****************Keep Patience******************\n");
	kprintf("\t================================================\n");
	SetBackColour(BLACK);	
	SetTextColour(RED);
	//show_memory_map(mbootinfo);	
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
	//enable();
	//disable();
	kprintf("Starting Memory Manager(very basic)\n");	
	init_phys_mem(mbootinfo);
	init_paging(total_mem);
	install_page_fault();
	enable();
	kprintf("\nGoodbye\n");
	
	/*kprintf("Trying a DIV 0\n");
	for( a=10000;a>1;a--);
	a=a/0; //generate a div 0 exception*/
	
	SetBackColour(BLACK);
	SetTextColour(BRIGHTGREEN);
	//int_ptr=(int*)kmalloc(200);
	//kprintf("int_ptr : 0x%x\n",int_ptr);
	probe_pci();
	pci_scan();
	
	init_floppy();
	kprintf("TODO:scheduler,threads OR processes, Shell,Few applications ,etc\n");
	//do_page_fault= *ptr;
	//setVideoMode(12);
	//box(100,100,400,400);
	//init_windows();
	//errmsgbox("Test Error$","Hello World this is not enough$");
	//displaymessages(100,100,"Hello World$",4);
	while(1);	
	halt();		
}

