#ifndef _PAGING_H
#define _PAGING_H

#define PAGE_SIZE	4096
#define PAGE_SZ 	PAGE_SIZE
#define PAGE_SHIFT 	12

#define PDE_SHIFT	22
#define PDE_MASK	0x3FF

#define PTE_SHIFT	PAGE_SHIFT
#define PTE_MASK        0x3FF

#define PAGE_ROUND_UP(x) ( (x&0xfff) ? ((x&(~0xfff))+0x1000) : x )
#define PAGE_ROUND(x) ((x) & 0xFFFFF000)

#define FLUSH_TLB()    set_cr3(get_cr3())

/* Intel constants */
#define PTE_VALID	0x00000001
#define PTE_WRITE	0x00000002
#define PTE_USER	0x00000004
#define PTE_WTHRU	0x00000008
#define PTE_NCACHE	0x00000010
#define PTE_REF		0x00000020
#define PTE_MOD		0x00000040
#define PTE_GLOBAL	0x00000100
#define PTE_AVAIL	0x00000E00
#define PTE_PFN		0xFFFFF000

#define PDE_VALID	0x00000001
#define PDE_WRITE	0x00000002
#define PDE_USER	0x00000004
#define PDE_WTHRU	0x00000008
#define PDE_NCACHE	0x00000010
#define PDE_REF		0x00000020
#define PDE_AVAIL	0x00000E00
#define PDE_PFN		0xFFFFF000

/* linear address to page number */
#define lin2pdenum(x)	(((x) >> PDE_SHIFT) & PDE_MASK)
#define lin2ptenum(x)	(((x) >> PTE_SHIFT) & PTE_MASK)

/* page number to address */
#define pdenum2lin(x)	((ulong)(x) << PDE_SHIFT)
#define ptenum2lin(x)	((ulong)(x) <> PTE_SHIFT)

static unsigned int get_CR0 (void)
{
  register unsigned int x;
  __asm__ __volatile__ ("movl %%cr0,%0" : "=r" (x));
  return (x);
}
static unsigned int get_CR2 (void)
{
  register unsigned int x;
  __asm__ __volatile__ ("movl %%cr2,%0" : "=r" (x));
  return (x);
}
static unsigned int get_CR3 (void)
{
  register unsigned int x;
  __asm__ __volatile__ ("movl %%cr3,%0" : "=r" (x));
  return (x);
}

#define set_CR0(c)  __asm__ __volatile__ ("movl %%eax, %%cr0"::"a" (c))
#define set_CR3(c)  __asm__ __volatile__ ("movl %%eax, %%cr3"::"a" (c))

static void flush_tlb(void) { set_CR3(get_CR3()); }
extern unsigned int kernelbase;


void init_paging(unsigned int avail);


#endif
