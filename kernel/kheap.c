/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  Heap implementation                                    |
|  Taken from :  Chris Ghiese(GazOS)                      |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#include "kheap.h"
#include "low-io.h"
#include "mutex.h"
//#include "stdio.h"
#include "vga.h"
#include "string.h"

#define	MALLOC_MAGIC	0x6D92	/* must be < 0x8000 */
extern unsigned int kend;
extern unsigned int total_mem;
static unsigned char *heap_bot,*heap_top;
unsigned int mem_avail=0;
unsigned int times_allocated=0;
unsigned int times_deallocated=0;
void init_heap()
{
	heap_bot=(unsigned char*)kend;
	
	heap_top=(unsigned char*)total_mem;
	kprintf("heap bottom %08x heap top %08x\n",kend,total_mem);
	malloc_t* m=(malloc_t*)heap_bot;
	if(m==NULL)
	{
		SetColour(RED,BLACK,0);
		kprintf("Could not initialize Heap\n halting the kernel\n");
		SetColour(BRIGHTWHITE,BLACK,0);		
		__asm__("cli");
		__asm__("hlt");
	}
	else
	{
		//claim all the available memory after kernel end. 
		m->magic= MALLOC_MAGIC;
		m->next=NULL; //nothing after this block
		m->size=heap_top-heap_bot-sizeof(malloc_t); //total memory available
		m->used=0;    //not used free/available
	}
	kprintf("Heap initialized\n");
	mem_avail=m->size;
}
void *kmalloc(size_t size)
{
	mutex_t km_mt;
	size_t required;
	malloc_t* n;
	init_mutex(&km_mt);
	mutex_try_lock(&km_mt);
	required=size+sizeof(malloc_t);	
	malloc_t* m=(malloc_t*)heap_bot;
	if(m)
	{
		//Ok
		if(m->magic!=MALLOC_MAGIC)
		{
			//corrupt or error or un-initialized
			SetColour(RED,BLACK,0);
			kprintf("Corrupted Heap\n");
			SetColour(BRIGHTWHITE,BLACK,0);
			mutex_unlock(&km_mt);
			return NULL;
		}
		while(m)
		{
			if(m->used==1)
			{
				m=m->next;
				continue;
			}
			if(m->size<required)
			{
				m=m->next;
				continue;
			}
			//we can allocate
			//cout<<"required= "<<required<<" available= "<<mem_avail<<"\n";
			n=m+sizeof(malloc_t)+size;
			n->magic=MALLOC_MAGIC;
			n->used=0;
			n->next=m->next;
			n->size=m->size-size-sizeof(malloc_t);
			m->used=1;
			m->next=n;
			m->size=size;
			mutex_unlock(&km_mt);
			mem_avail-=required;
			times_allocated++;
			return ((char*)m+sizeof(malloc_t));
		}
		
	}
	//m is NULL
	mutex_unlock(&km_mt);
	SetColour(RED,BLACK,0);
	kprintf("No Memory for allocation %u available\n",mem_avail);
	SetColour(BRIGHTWHITE,BLACK,0);
	dump_heap();
	
	return NULL;	
}
void kfree(void *p)
{
	malloc_t *m;
	mutex_t kf_mt;
	init_mutex(&kf_mt);
	mutex_try_lock(&kf_mt);
	m = (malloc_t *)((char *)p - sizeof(malloc_t));
	if(((unsigned char*)p< heap_bot)||((unsigned char *)p>heap_top))
	{
		kprintf("Illigal memory area %08x\n",(unsigned int)p);
		mutex_unlock(&kf_mt);
		return;
	}
	//so p is within our limit
	if(m->magic!=MALLOC_MAGIC)
	{
		kprintf("*** Bad Magic / corrupted heap\n");
		kprintf("*** Go consult a scientist :{\n");
		mutex_unlock(&kf_mt);
		return;
	}
	
	if(m->used==0)
	{
		//unused memory
		kprintf("***Trying to free unused Unused memory\n");
		mutex_unlock(&kf_mt);
		return;
	}
	//if we reached here means we are to mark the block as free
	m->used=0;		
	//try unfragment the heap
	m=(malloc_t*)heap_bot;
	for(;m!=NULL;m=m->next)
	{	
		while(!m->used && m->next != NULL && !m->next->used)
		{
/* resize this block */
			m->size += sizeof(malloc_t) + m->next->size;
/* merge with next block */
			m->next = m->next->next;
		}
	}
	times_deallocated++;
	mutex_unlock(&kf_mt);
}
void *krealloc(void *blk, size_t size)
{
	void *new_blk;
	malloc_t *m;

/* size == 0: free block */
	if(size == 0)
	{
		if(blk != NULL)
			kfree(blk);
		new_blk = NULL;
	}
	else
	{
/* allocate new block */
		new_blk = kmalloc(size);
/* if allocation OK, and if old block exists, copy old block to new */
		if(new_blk != NULL && blk != NULL)
		{
			m = (malloc_t *)((char *)blk - sizeof(malloc_t));
			if(m->magic != MALLOC_MAGIC)
			{
				kprintf("*** attempt to krealloc() block at %016x with bad magic value\n",(unsigned int)blk);
				return NULL;
			}
/* copy minimum of old and new block sizes */
			if(size > m->size)
				size = m->size;
			memcpy(new_blk, blk, size);
/* free the old block */
			kfree(blk);
		}
	}
	return new_blk;
}
void dump_heap()
{
	unsigned blks_used = 0, blks_free = 0;
	size_t bytes_used = 0, bytes_free = 0;
	malloc_t *m;
	int total;
	kprintf("===============================================\n");
	for(m = (malloc_t *)heap_bot; m != NULL; m = m->next)
	{
		kprintf("blk : %08x - %4d bytes ",(unsigned int)m,m->size);
		if(m->used)
		{
			blks_used++;
			bytes_used += m->size;
			kprintf("used\n");
		}
		else
		{
			blks_free++;
			bytes_free += m->size;
			kprintf("free\n");
		}
	}
	kprintf("blks:  %8u used %8u free %8u total\n",blks_used,blks_free,blks_used + blks_free);
	kprintf("bytes: %8u used %8u free %8u total\n",bytes_used,bytes_free,bytes_used + bytes_free);
	
	total = (bytes_used + bytes_free) +
			(blks_used + blks_free) * sizeof(malloc_t);
	if(total != heap_top - heap_bot)
		kprintf("*** some heap memory is not accounted for\n");
	kprintf("malloc called %4d and free called %4d times\n",times_allocated,times_deallocated);
	kprintf("===============================================\n");
	
}
