#include "paging.h"
#include "idt.h"
#include "low-io.h"
#include "string.h"
#define MAX_MEMORY 128*1024*1024 //this is a small system and 128MB is sufficient
unsigned int CR3;
static unsigned int total_memory;
void init_paging(unsigned int available)
{
	unsigned int *PD, *PT;
	unsigned int CR0;	
	int i, tables = 0;

	CR3 = (unsigned int) phys_alloc_page_hi(); //allocates a page above 1MB
	PD = (unsigned int *) CR3;
	PT = (void *)0;
	/* do not map more than 128Mb of physical memory */
	if (available > MAX_MEMORY) total_memory = MAX_MEMORY;
	else total_memory = available;
	kprintf("\tPaging\t");
	for(i=0; i<(total_memory >> 12); i++) 
	{
		if ((i & 0x3FF) == 0) 
		{
			PT = phys_alloc_page_hi();
			tables++;
	 		PD[i>>10] = (unsigned int)PT + PDE_VALID;
      		}
      		PT[i & 0x3FF] = (i << 12) + PTE_VALID;
   	}
	CR0 = get_CR0();
	set_CR3(CR3);
   /* set most significant bit of CR0 */
	set_CR0(CR0 | 0x80000000);
	kprintf("enabled\n"); 
}
void page_fault(regs *r)
{
	unsigned int faulting_address;
	char buf[100];
	memset(buf,0,100);
	faulting_address=get_CR2();
    
	// The error code gives us details of what happened.
	int present   = !(r->err_code & 0x1); // Page not present
	int rw = r->err_code & 0x2;           // Write operation?
	int us = r->err_code & 0x4;           // Processor was in user-mode?
	int reserved = r->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
	int id = r->err_code & 0x10;          // Caused by an instruction fetch?
	if (present)
	kprintf("present ");
	if (rw)
	kprintf("read-only ");
	if (us)
	kprintf("user mode ");
	if(reserved)
	kprintf("reserved ");
	if(id)
	kprintf("instruction fetch ");
	kprintf("Pagefault happened  at 0x%x\n",faulting_address);
	halt();
}
void install_page_fault()
{
	isr_install_handler(14,page_fault);
}
/* note: you should not use this with (delta % PAGE_SIZE)!=0 */
// delta is in bytes
// returns the address of new heap
// this is a heavily modified version of actual code from littleOS
void *ksbrk(unsigned int delta)
{
	unsigned int addr;
	static unsigned int current_break;
	current_break=phys_alloc_page_hi();  // start at the next available page
	
        if (delta == 0) return (void *) (current_break);
	if (delta % PAGE_SIZE || delta < 0) return (void *) -1;

	addr = current_break;
        while (delta > 0) {
                /* reserve physical page (mark it allocated) */
		phys_mark_page(current_break, 1);

       		current_break += PAGE_SIZE;
               	delta -= PAGE_SIZE;
        }
	if (current_break > total_memory) return (void *) -1;

        return (void *) (addr);
}

