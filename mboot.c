#include "mboot.h"

unsigned long get_available_memory(multibootInfo *bootInfo)
{
  if (bootInfo->flags & MULTIBOOT_MEMORY) {
      return (unsigned long)((bootInfo->memoryUpper + 1024) << 10); 
  } else {
      /* this shouldn't happen - the bootloader has
       * to provide information on available memory */

      kprintf("No memory information!\n");
	return 0;
  }
}
unsigned long get_used_memory(multibootInfo *bootInfo)
{
  int i;
  extern void end;
  unsigned long top = (unsigned long)&end;

  if (bootInfo->flags & MULTIBOOT_MODS) {
      for (i=0; i<bootInfo->modulesCount; i++)
        if (bootInfo->modules[i].end > top)
          top = bootInfo->modules[i].end;
  }

  top = (top + 4095) & 0xFFFFF000;
  return (unsigned long)top;
}
void show_memory_map(multibootInfo *bootInfo)
{
  int i,parts;
  if (bootInfo->flags & MULTIBOOT_MEM_MAP) {
    kprintf("GRUB: BIOS provided physical RAM map\n");
    parts=bootInfo->mmap_length/sizeof(memoryMap);
    for (i=0;i<parts;i++) {
      kprintf("GRUB: addr: %08x%08x  len: %08x%08x ",
      bootInfo->mmap_addr[i].BaseAddrHigh,
      bootInfo->mmap_addr[i].BaseAddrLow,
      bootInfo->mmap_addr[i].LengthHigh,
      bootInfo->mmap_addr[i].LengthLow);
      kprintf("%s\n",(1==bootInfo->mmap_addr[i].Type)?"(usable)":"(reserved)");
    }
    kprintf("\n");
  }
}
void show_elf_info(multibootInfo *btinfo)
{
	kprintf("ELF Infos *** ??? ***\n");
	kprintf("num sects => %l \n",btinfo->num);
	kprintf("size      => %l \n",btinfo->size);
	kprintf("address   => 0x%x \n",btinfo->addr);
}
