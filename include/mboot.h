/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  Multiboot defination                                   |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#ifndef __MBOOT_H__
#define __MBOOT_H__

#define MULTIBOOT_MEMORY	(1L<<0)
#define MULTIBOOT_BOOT_DEVICE	(1L<<1)
#define MULTIBOOT_CMDLINE	(1L<<2)
#define MULTIBOOT_MODS		(1L<<3)
#define MULTIBOOT_AOUT_SYMS	(1L<<4)
#define MULTIBOOT_ELF_SHDR	(1L<<5)
#define MULTIBOOT_MEM_MAP	(1L<<6)
typedef struct multibootModule
{
  /* physical start and end addresses of the module data itself.  */
  unsigned long start;
  unsigned long end;

  /* arbitrary ASCII-Z string associated with the module.  */
  char *string;

  /* boot loader must set to 0; OS must ignore.  */
  unsigned long reserved;
} __attribute__ ((packed)) multibootModule;

typedef struct memoryMap
{
  unsigned long Size;
  unsigned long BaseAddrLow;
  unsigned long BaseAddrHigh;
  unsigned long LengthLow;
  unsigned long LengthHigh;
  unsigned long Type;
} __attribute__ ((packed)) memoryMap;

typedef struct multibootHeader
{
	unsigned long header_magic;    //multiboot header magic 0x1BADB002
	unsigned long header_flags;    //flags enabled 
	unsigned long ckecksum;        //checksum
	unsigned long header_address;  //the location of this structure
	unsigned long kernel_start;    // start of kernel .text section
	unsigned long kernel_data_end; //end of kernel .bss
	unsigned long kernel_end;      // kernel end address
	unsigned long kernel_entry;    //kernel entry point
} __attribute__ ((packed)) multibootHeader;
extern multibootHeader mboot; //from startup.asm

typedef struct multibootInfo
{
  /* these flags indicate which parts of the multiboot_info are valid;
   * see below for the actual flag bit definitions.  */
  unsigned long flags;

  /* lower/upper memory installed in the machine.
   * valid only if MULTIBOOT_MEMORY is set in flags word above.  */
  unsigned long memoryLower;
  unsigned long memoryUpper;

  /* BIOS disk device the kernel was loaded from.
   * Valid only if MULTIBOOT_BOOT_DEVICE is set in flags word above.  */
  unsigned char bootDevice[4];

  /* command-line for the OS kernel: a null-terminated ASCII string.
   * valid only if MULTIBOOT_CMDLINE is set in flags word above.  */
  char *commandLine;

  /* list of boot modules loaded with the kernel.
   * valid only if MULTIBOOT_MODS is set in flags word above.  */
  unsigned long modulesCount;
  multibootModule *modules;

  /* info about kernel ELF file */
  unsigned long num;
  unsigned long size;
  unsigned long addr;
  unsigned long reserved;

  /* memory map -- int 0x15 BIOS call (e820) */
  unsigned long mmap_length;
  memoryMap *mmap_addr;
} __attribute__ ((packed)) multibootInfo;

unsigned int get_kernel_start();
unsigned int get_kernel_end();
void show_memory_map(multibootInfo *bootInfo);
unsigned long get_available_memory(multibootInfo *bootInfo);
unsigned long get_used_memory(multibootInfo *bootInfo);
void show_elf_info(multibootInfo *btinfo);
char *get_boot_dev(multibootInfo *btinfo);

#endif
