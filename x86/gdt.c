/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  GDT Implementation                                     |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#include "gdt.h"
#include "low-io.h"

#define dword unsigned int
#define byte unsigned char
DESCR_SEG gdt[3]; /* we will have only 4 segments null, k-code,k-data,k-stack*/
GDTR gdtr;

/* Creates a descriptor segment for a GDT*/
void setup_GDT_entry (DESCR_SEG *item, dword base, dword limit, byte access, byte attribs)
{
	item->base_l = base & 0xFFFF;
	item->base_m = (base >> 16) & 0xFF;
	item->base_h = base >> 24;
	item->limit = limit & 0xFFFFFFFF;
	item->attribs = attribs | ((limit >> 16) & 0x0F);
	item->access = access;
}
void lgdt(GDTR *mgdtr)
{
	__asm__ __volatile__ ("lgdt (%0)"::"p"(mgdtr));
	
	__asm__ __volatile__("movl $0x10, %eax\n"
          "movw %ax, %ss\n"
          "movw %ax, %ds\n"
          "movw %ax, %es\n"
          "movw %ax, %fs\n"
          "movw %ax, %gs");
	__asm__ __volatile__("ljmp $8, $__flush2\n"
          "__flush2:");
}
void setup_GDT()
{
	/* 0x00 -- null descriptor */
  	setup_GDT_entry (&gdt[0], 0, 0, 0, 0);

  	/* 0x08 -- code segment descriptor */
  	setup_GDT_entry (&gdt[1], 0, 0xFFFFFFFF, ACS_CODE, 0xCF);

  	/* 0x10 -- data segment descriptor */
  	setup_GDT_entry (&gdt[2], 0, 0xFFFFFFFF, ACS_DATA, 0xCF);

  	/* 0x18 -- stack segment descriptor */
  //	setup_GDT_entry (&gdt[3], 0, 0xFFFFFFFF, ACS_STACK, 0);
	/* 0x20 -- video segment */
//	setup_GDT_entry (&gdt[4],0xB800,0xffff,ACS_DATA,0);
	gdtr.base=(unsigned long) &gdt;
	gdtr.limit=(sizeof(DESCR_SEG) * 3) - 1;
	/* GDT loaded*/
        lgdt(&gdtr);
}
