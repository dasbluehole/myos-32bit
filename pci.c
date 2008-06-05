/*PCI Test code*/
extern void putch(char c);
extern void kprintf(const char *fmt,...);
struct pci_sig
	{
		unsigned char sig[4];
		void *entry;
		unsigned char rev_lev;
		unsigned char length;
		unsigned char cksum;
		unsigned char res[5];
	}__attribute__ ((packed));
void probe_pci()
{
	char *rom_beg=0xe0000;
	char *rom_end=0xFFFFF;
	struct pci_sig *pci=rom_beg;
	do{
		
		if(strncmp(pci->sig,"_32_",4)==0)
		{
			int i;			
			for (i=0;i<4;i++)
			putch(pci->sig[i]);			
			kprintf("PCI BIOS found at %p\t",pci);
			kprintf("entry point at %p\n",pci->entry);
		}
		pci+=16;
	}while(pci<0xfffff);
}

