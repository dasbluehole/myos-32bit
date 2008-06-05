/* physical memory mapping functons and structures.
*/
#include "mboot.h"
#include "bitops.h"
#include "paging.h"

#define PAGE_SIZE 4096
#define MAX_MEMORY 128*1024*1024 //this is a small system and 128MB is sufficient
#define MAX_PAGES (MAX_MEMORY/PAGE_SIZE) //maximum number of pages we have
#define MAX_BITS (MAX_PAGES/32) //Number of integers we need to reperesent all the pages, 1-bit per page
unsigned int mem_map[MAX_BITS]; //this is the bit-map for our physical memory

unsigned long total_mem; //total memory we got from the GRUB

unsigned int num_pages; //total number of  pages 

extern unsigned end; //linker script will provide this. and also page alligned.
extern unsigned start;// -do- our kernel entry
//allocates a page by setting the coresponding bit in bit-map
void alloc_page(unsigned int page_num)
{
#ifdef DEBUG
	if(test_bit(page_num,&mem_map))
	{
		unsigned int i;
		kprintf("WARNING: page %d is already allocated\n",page_num);
		for(i=0;i<1000;i++);
	}
#endif
	set_bit(page_num,&mem_map);
}
//frees a page by unseting coresponding bit in bit-map
void free_page(unsigned int page_num)
{
#ifdef DEBUG	
	if(!test_bit(page_num,&mem_map))
	{
		unsigned int i;
		kprintf("WARNING: page %d is already freed\n",page_num);
		for(i=0;i<1000;i++);
	}
#endif
	clear_bit(page_num,&mem_map);
}
//checks if coresponding bit for page is unset
unsigned char is_page_free(page_num)
{
	return !test_bit(page_num, &mem_map);
}

//finds the first free page
int find_free_page(unsigned int start)
{
	int l;
        for (l = lin2ptenum(start); l < num_pages; l++)
        	if (is_page_free(l)) {
                	return l*PAGE_SIZE;
                }
	return (int)-1;
}
//upper level functions
static unsigned int start_alloc=0;
//allocate a page and returns the address
unsigned int phys_alloc_page()
{
	unsigned int addr;
	addr=find_free_page(start_alloc);
	if (addr==(unsigned int)-1) //if find_free_page fails
		return addr;   // error allocating a free page
	start_alloc=addr;      // else
	alloc_page(lin2ptenum(addr)); //allocate a page
	return addr;                  //return the address  
}
static unsigned int hi_start=0x100000;
unsigned int phys_alloc_page_hi()
{
	unsigned int addr;
	addr=find_free_page(hi_start);
	if(addr==(unsigned int)-1)
		return addr;
	hi_start=addr;
	alloc_page(lin2ptenum(addr));
	return addr;
}
	
// free the physical page at address
void pys_free_page(unsigned int addr)
{
	unsigned int index = PAGE_ROUND(addr) / PAGE_SIZE;
	if (index < num_pages && !is_page_free(index)) {
        	free_page(index);
                start_alloc = index * PAGE_SIZE;
	}
}
//marks a perticular page as free or allocated
void phys_mark_page(unsigned int addr, int what)
{
	if (what == 0) free_page(PAGE_ROUND(addr) / PAGE_SIZE);
        else
        if (what == 1) alloc_page(PAGE_ROUND(addr) / PAGE_SIZE);
}
//gets number of free pages
unsigned int get_free_pages(void)
{
	unsigned int i, j;
        j = 0;
        for (i = 0; i < num_pages; i++)
        	if (is_page_free(i)) j++;
	return j;
}
unsigned phys_base;
void init_phys_mem(multibootInfo *mbi)
{
	unsigned int k_start,k_end,j;	
	total_mem = mbi->memoryUpper * 1024 + 0x100000; //bytes
	if(total_mem > MAX_MEMORY) total_mem=MAX_MEMORY;
	num_pages= total_mem/PAGE_SIZE;
	k_start=(unsigned int)&start -0x20; //the multiboot header is 32 bytes
	k_end=(unsigned int)&end;
	phys_base=k_start;
	kprintf("Physical Memory \n\t");	
	kprintf("Memory Total %d kb\n\t Free %d kb\n\t kernel takes %d kb\n",total_mem/1024,(total_mem-k_end \
		+k_start)/1024,(k_end-k_start)/ 1024);
	kprintf("First we will mark all the pages as free\t");
	for (j = 0; j < MAX_PAGES; j++) mem_map[j] = 0;
	kprintf("done\n");
	kprintf("Mark the reserved Pages(BIOS/Video)\t");
	for (j = 0xA0000; j < 0x100000; j+=PAGE_SIZE)
        	alloc_page(j/PAGE_SIZE);
	kprintf("done\n");
	kprintf("Marking Pages used by Kernel\t");
	for (j = 0; j < PAGE_ROUND_UP((long)&end); j+= PAGE_SIZE)
        	alloc_page((j+phys_base)/PAGE_SIZE);
	kprintf("done\n");
	
}

