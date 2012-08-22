/*PCI Test code*/
#include "pci.h"
#include "low-io.h"
#include "stdio.h"
#include "string.h"
#include "kheap.h"

//! The amount of supported PCI classes.
#define PCI_CLASS_ENTRIES	( sizeof(classes)/sizeof(classes_t) )
#define L1_CACHE_BYTES			(1 << 4) //for 386
//! PCI supported classes. It is used to recognize to what
//! class a device belongs.
classes_t classes[] =
{
    { 0x00, 0x00, 0x00, "Undefined" },
    { 0x00, 0x01, 0x00, "VGA" },

    { 0x01, 0x00, 0x00, "SCSI" },
    { 0x01, 0x01, 0x00, "IDE" },
    { 0x01, 0x01, 0x8A, "Controller IDE via Bus Master" },
    { 0x01, 0x02, 0x00, "Floppy" },
    { 0x01, 0x03, 0x00, "IPI" },
    { 0x01, 0x04, 0x00, "RAID" },
    { 0x01, 0x05, 0x20, "ATA (Single DMA)" },
    { 0x01, 0x05, 0x30, "ATA (Chained DMA)" },
    { 0x01, 0x80, 0x00, "Other" },

    { 0x02, 0x00, 0x00, "Ethernet" },
    { 0x02, 0x01, 0x00, "Token Ring" },
    { 0x02, 0x02, 0x00, "FDDI" },
    { 0x02, 0x03, 0x00, "ATM" },
    { 0x02, 0x04, 0x00, "ISDN" },
    { 0x02, 0x05, 0x00, "WordFip" },
    { 0x02, 0x06, 0x00, "PICMG 2.14" },
    { 0x02, 0x80, 0x00, "Other" },

    { 0x03, 0x00, 0x00, "VGA" },
    { 0x03, 0x00, 0x01, "VGA+8514" },
    { 0x03, 0x01, 0x00, "XGA" },
    { 0x03, 0x02, 0x00, "3D" },
    { 0x03, 0x80, 0x00, "Other" },

    { 0x04, 0x00, 0x00, "Video" },
    { 0x04, 0x01, 0x00, "Audio" },
    { 0x04, 0x02, 0x00, "Telephony" },
    { 0x04, 0x80, 0x00, "Other" },

    { 0x05, 0x00, 0x00, "RAM" },
    { 0x05, 0x01, 0x00, "Flash" },
    { 0x05, 0x80, 0x00, "Other" },

    { 0x06, 0x00, 0x00, "PCI to HOST" },
    { 0x06, 0x01, 0x00, "PCI to ISA" },
    { 0x06, 0x02, 0x00, "PCI to EISA" },
    { 0x06, 0x03, 0x00, "PCI to MCA" },
    { 0x06, 0x04, 0x00, "PCI to PCI" },
    { 0x06, 0x04, 0x01, "PCI to PCI (Subtractive Decode)" },
    { 0x06, 0x05, 0x00, "PCI to PCMCIA" },
    { 0x06, 0x06, 0x00, "PCI to NuBUS" },
    { 0x06, 0x07, 0x00, "PCI to Cardbus" },
    { 0x06, 0x08, 0x00, "PCI to RACEway (Transparent)" },
    { 0x06, 0x08, 0x01, "PCI to RACEway (End-point)" },
    { 0x06, 0x09, 0x00, "PCI to PCI" },
    { 0x06, 0x09, 0x40, "PCI to PCI (Primary bus to host)" },
    { 0x06, 0x09, 0x80, "PCI to PCI (Secondary bus to host)" },
    { 0x06, 0x0A, 0x00, "PCI to InfiBand" },
    { 0x06, 0x80, 0x00, "PCI to Other" },

    { 0x07, 0x00, 0x00, "Serial" },
    { 0x07, 0x00, 0x01, "Serial - 16450" },
    { 0x07, 0x00, 0x02, "Serial - 16550" },
    { 0x07, 0x00, 0x03, "Serial - 16650" },
    { 0x07, 0x00, 0x04, "Serial - 16750" },
    { 0x07, 0x00, 0x05, "Serial - 16850" },
    { 0x07, 0x00, 0x06, "Serial - 16950" },
    { 0x07, 0x01, 0x00, "Parallel" },
    { 0x07, 0x01, 0x01, "Parallel - BiDir" },
    { 0x07, 0x01, 0x02, "Parallel - ECP" },
    { 0x07, 0x01, 0x03, "Parallel - IEEE1284" },
    { 0x07, 0x01, 0xFE, "Parallel - IEEE1284 Target" },
    { 0x07, 0x02, 0x00, "Multiport Serial" },
    { 0x07, 0x03, 0x00, "Hayes Compatible Modem" },
    { 0x07, 0x03, 0x01, "Hayes Compatible Modem, 16450" },
    { 0x07, 0x03, 0x02, "Hayes Compatible Modem, 16550" },
    { 0x07, 0x03, 0x03, "Hayes Compatible Modem, 16650" },
    { 0x07, 0x03, 0x04, "Hayes Compatible Modem, 16750" },
    { 0x07, 0x80, 0x00, "Other" },

    { 0x08, 0x00, 0x00, "PIC" },
    { 0x08, 0x00, 0x01, "ISA PIC" },
    { 0x08, 0x00, 0x02, "EISA PIC" },
    { 0x08, 0x00, 0x10, "I/O APIC" },
    { 0x08, 0x00, 0x20, "I/O(x) APIC" },
    { 0x08, 0x01, 0x00, "DMA" },
    { 0x08, 0x01, 0x01, "ISA DMA" },
    { 0x08, 0x01, 0x02, "EISA DMA" },
    { 0x08, 0x02, 0x00, "Timer" },
    { 0x08, 0x02, 0x01, "ISA Timer" },
    { 0x08, 0x02, 0x02, "EISA Timer" },
    { 0x08, 0x03, 0x00, "RTC" },
    { 0x08, 0x03, 0x01, "ISA RTC" },
    { 0x08, 0x04, 0x00, "Hot-Plug" },
    { 0x08, 0x80, 0x00, "Other" },

    { 0x09, 0x00, 0x00, "Keyboard" },
    { 0x09, 0x01, 0x00, "Pen" },
    { 0x09, 0x02, 0x00, "Mouse" },
    { 0x09, 0x03, 0x00, "Scanner" },
    { 0x09, 0x04, 0x00, "Game Port" },
    { 0x09, 0x04, 0x10, "Game Port (Legacy)" },
    { 0x09, 0x80, 0x00, "Other" },

    { 0x0a, 0x00, 0x00, "Generic" },
    { 0x0a, 0x80, 0x00, "Other" },

    { 0x0b, 0x00, 0x00, "386" },
    { 0x0b, 0x01, 0x00, "486" },
    { 0x0b, 0x02, 0x00, "Pentium" },
    { 0x0b, 0x03, 0x00, "PentiumPro" },
    { 0x0b, 0x10, 0x00, "DEC Alpha" },
    { 0x0b, 0x20, 0x00, "PowerPC" },
    { 0x0b, 0x30, 0x00, "MIPS" },
    { 0x0b, 0x40, 0x00, "Coprocessor" },
    { 0x0b, 0x80, 0x00, "Other" },

    { 0x0c, 0x00, 0x00, "FireWire" },
    { 0x0c, 0x00, 0x10, "OHCI FireWire" },
    { 0x0c, 0x01, 0x00, "Access.bus" },
    { 0x0c, 0x02, 0x00, "SSA" },
    { 0x0c, 0x03, 0x00, "USB (UHCI)" },
    { 0x0c, 0x03, 0x10, "USB (OHCI)" },
    { 0x0c, 0x03, 0x20, "USB (EHCI)" },
    { 0x0c, 0x03, 0x80, "USB" },
    { 0x0c, 0x03, 0xFE, "USB Device" },
    { 0x0c, 0x04, 0x00, "Fiber" },
    { 0x0c, 0x05, 0x00, "SMBus Controller" },
    { 0x0c, 0x06, 0x00, "InfiniBand" },
    { 0x0c, 0x80, 0x00, "Other" },

    { 0x0d, 0x00, 0x00, "iRDA" },
    { 0x0d, 0x01, 0x00, "Consumer IR" },
    { 0x0d, 0x10, 0x00, "RF" },
    { 0x0d, 0x80, 0x00, "Other" },

    { 0x0e, 0x00, 0x00, "I2O" },
    { 0x0e, 0x80, 0x00, "Other" },

    { 0x0f, 0x01, 0x00, "TV" },
    { 0x0f, 0x02, 0x00, "Audio" },
    { 0x0f, 0x03, 0x00, "Voice" },
    { 0x0f, 0x04, 0x00, "Data" },
    { 0x0f, 0x80, 0x00, "Other" },

    { 0x10, 0x00, 0x00, "Network" },
    { 0x10, 0x10, 0x00, "Entertainment" },
    { 0x10, 0x80, 0x00, "Other" },

    { 0x11, 0x00, 0x00, "DPIO Modules" },
    { 0x11, 0x01, 0x00, "Performance Counters" },
    { 0x11, 0x10, 0x00, "Comm Sync, Time+Frequency Measurement" },
    { 0x11, 0x20, 0x00, "Management Card" },
    { 0x11, 0x80, 0x00, "Other" },
};

void delay(unsigned char de)
{
	int i,j;
	for(i=0;i<10;i++)
	{
		for(j= de;j>0;j--)
		{
			
		}
  	}
}
/* reads PCI configuration byte/word/dword
   from the bus dev func reg and sizeof the read opperation
*/

unsigned int read_pci(int bus, int dev, int func, int reg, int uint8_ts)
{
	unsigned short base;

	union {
		confadd_t c;
		unsigned int n;
	} u;

	u.n = 0;
	u.c.enable = 1;
	u.c.rsvd = 0;
	u.c.bus = bus;
	u.c.dev = dev;
	u.c.func = func;
	u.c.reg = reg & 0xFC;

	outportl(0xCF8, u.n);
	base = 0xCFC + (reg & 0x03);

	switch(uint8_ts)
	{
		case 1: return inportb(base);
		case 2: return inportw(base);
		case 4: return inportl(base);
		default: return 0;
	}
}
/* writes a config byte/word/dword to 
   bus device function register with v= value to write and size of the write operation
*/
void write_pci(int bus, int dev, int func, int reg, unsigned int v, int uint8_ts)
{
	unsigned short base;

	union {
		confadd_t c;
		unsigned int n;
	} u;

	u.n = 0;
	u.c.enable = 1;
	u.c.rsvd = 0;
	u.c.bus = bus;
	u.c.dev = dev;
	u.c.func = func;
	u.c.reg = reg & 0xFC;

	base = 0xCFC + (reg & 0x03);
	outportl(0xCF8, u.n);

	switch(uint8_ts)
	{
		case 1: outportb(base, (unsigned char) v); break;
		case 2: outportw(base, (unsigned short) v); break;
		case 4: outportl(base, v); break;
	}
}
/*wraper functions for read and write*/
unsigned char pci_read_config_byte(int bus, int dev, int func, int reg)
{
	return ( read_pci(bus, dev, func, reg, sizeof(unsigned char)) );
}
unsigned short pci_read_config_word(int bus, int dev, int func, int reg)
{
	return ( read_pci(bus, dev, func, reg, sizeof(unsigned short)) );
}
unsigned int pci_read_config_dword(int bus, int dev, int func, int reg)
{
	return ( read_pci(bus, dev, func, reg, sizeof(unsigned int)) );
}
void pci_write_config_byte(int bus, int dev, int func, int reg, unsigned char val)
{
	write_pci(bus, dev, func, reg, val, sizeof(unsigned char));
}
void pci_write_config_word(int bus, int dev, int func, int reg, unsigned short val)
{
	write_pci(bus, dev, func, reg, val, sizeof(unsigned short));
}
void pci_write_config_dword(int bus, int dev, int func, int reg, unsigned int val)
{
	write_pci(bus, dev, func, reg, val, sizeof(unsigned int));
}
//==============================================================================
// checks if PCI bus is present
static bool is_pci_present()
{
//	taken from http://www.waste.org/~winkles/hardware/pci.htm
	unsigned int res=inportl(PCI_DATA_REG);
#ifdef __DEBUG_
	kprinf("PCI BUS present\n");
#endif
	if(res==0xFFFFFFFF)
		return false;
	else
		return true;
}
/* reads the IRQ line for the device if present*/
void pci_read_irq(pci_cfg_t *cfg)
{
	unsigned char irq;

	irq = pci_read_config_byte(cfg->bus, cfg->dev, cfg->func, PCI_INTERRUPT_PIN);
	if (irq)
		irq = pci_read_config_byte(cfg->bus, cfg->dev, cfg->func, PCI_INTERRUPT_LINE);

	cfg->irq = irq;
}

/* calculates the size of the IO space for the device
   base is base address and mask is address mask
*/

unsigned int pci_size(unsigned int base, unsigned long mask)
{
	// Find the significant bits					//
	unsigned int size = mask & base;
	// Get the lowest of them to find the decode size		//
	size = size & ~(size-1);
	// extent = size - 1						//
	return(size-1);
}
/* reads the base address for a device
   number of bases to read rom address register	
*/   
void pci_read_bases(pci_cfg_t *cfg, int tot_bases, int rom)
{
	unsigned int l, sz, reg;
	int i;

	// Clear all previous bases and sizes informations		//
	memset(cfg->base, 0, sizeof(cfg->base));
	memset(cfg->size, 0, sizeof(cfg->size));
	memset(cfg->type, 0, sizeof(cfg->type));

	// Read informations about bases and sizes			//
	for(i=0; i<tot_bases; i++)
	{
		// Read bases and size					//
		reg = PCI_BASE_ADDRESS_0 + (i << 2);
		l = pci_read_config_dword(cfg->bus, cfg->dev, cfg->func, reg);
		pci_write_config_dword(cfg->bus, cfg->dev, cfg->func, reg, ~0);
		sz = pci_read_config_dword(cfg->bus, cfg->dev, cfg->func, reg);
		pci_write_config_dword(cfg->bus, cfg->dev, cfg->func, reg, l);

		// Check if informations are valid			//
		if (!sz || sz == 0xFFFFFFFF)
			continue;
		if (l == 0xFFFFFFFF)
			l = 0;

		// Store informations					//
		if ( (l & PCI_BASE_ADDRESS_SPACE) == PCI_BASE_ADDRESS_SPACE_MEMORY )
		{
			cfg->base[i] = l & PCI_BASE_ADDRESS_MEM_MASK;
			cfg->size[i] = pci_size(sz, PCI_BASE_ADDRESS_MEM_MASK);
			cfg->type[i] = PCI_IO_RESOURCE_MEM;
		}
		else
		{
			cfg->base[i] = l & PCI_BASE_ADDRESS_IO_MASK;
			cfg->size[i] = pci_size(sz, PCI_BASE_ADDRESS_IO_MASK);
			cfg->type[i] = PCI_IO_RESOURCE_IO;
		}
	}

	// --- ROM ---							//
	if (rom)
	{
		// Initialize values					//
		cfg->rom_base = 0;
		cfg->rom_size = 0;

		l = pci_read_config_dword(cfg->bus, cfg->dev, cfg->func, rom);
		pci_write_config_dword(cfg->bus, cfg->dev, cfg->func, rom, ~PCI_ROM_ADDRESS_ENABLE);
		sz = pci_read_config_dword(cfg->bus, cfg->dev, cfg->func, rom);
		pci_write_config_dword(cfg->bus, cfg->dev, cfg->func, rom, l);
		if (l == 0xFFFFFFFF)
			l = 0;
		if (sz && sz != 0xFFFFFFFF)
		{
			cfg->rom_base = l & PCI_ROM_ADDRESS_MASK;
			sz = pci_size(sz, PCI_ROM_ADDRESS_MASK);
			cfg->rom_size = cfg->rom_size + (unsigned long)sz;
		}
	}
}
/* probe for a PCI device 
   in bus device number function
*/
int pci_probe(int bus, int dev, int func, pci_cfg_t *cfg)
{
	unsigned int *temp = (unsigned int *) cfg;
	int i;

	for(i=0; i<4; i++)
		temp[i] = pci_read_config_dword(bus, dev, func, (i << 2));

	if((cfg->vendor_id == 0xFFFF)||(cfg->vendor_id == 0x0000))
		 return 0;

	// Setup the bus, device and function number			//
	cfg->bus = bus;
	cfg->dev = dev;
	cfg->func = func;

	// Set the power state to unknown				//
	cfg->current_state = 4;

	// Identify the type of the device				//
	switch(cfg->header_type & 0x7F)
	{
		case PCI_HEADER_TYPE_NORMAL:
		// --- NORMAL DEVICE ---				//
		// Read the IRQ line					//
		pci_read_irq(cfg);
		// Read the base memory and I/O addresses		//
		pci_read_bases(cfg, 6, PCI_ROM_ADDRESS);
		// Read subsysem vendor and subsystem device id		//
		cfg->subsys_vendor = pci_read_config_word(bus, dev, func, PCI_SUBSYSTEM_VENDOR_ID);
		cfg->subsys_device = pci_read_config_word(bus, dev, func, PCI_SUBSYSTEM_ID);
		break;

		case PCI_HEADER_TYPE_BRIDGE:
		// --- PCI <-> PCI BRIDGE ---				//
		pci_read_bases(cfg, 2, PCI_ROM_ADDRESS_1);
		break;

		case PCI_HEADER_TYPE_CARDBUS:
		// --- PCI CARDBUS ---					//
		// Read the IRQ line					//
		pci_read_irq(cfg);
		// Read the base memory and I/O addresses		//
		pci_read_bases(cfg, 1, 0);
		// Read subsysem vendor and subsystem device id		//
		cfg->subsys_vendor = pci_read_config_word(bus, dev, func, PCI_CB_SUBSYSTEM_VENDOR_ID);
		cfg->subsys_device = pci_read_config_word(bus, dev, func, PCI_CB_SUBSYSTEM_ID);
		break;

		default:
		// --- UNKNOW HEADER TYPE ---				//
		break;
	}

	return 1;
}

unsigned int pcibios_max_latency=255;
/* Enable bus mastering 32-bit DMA mode for the device
*/
void pci_set_master(pci_cfg_t *cfg)
{
	unsigned short cmd;
	unsigned char lat;

	cmd = pci_read_config_word(cfg->bus, cfg->dev, cfg->func, PCI_COMMAND);
	if ( !(cmd & PCI_COMMAND_MASTER) )
	{
		kprintf("PCI: Enabling bus mastering for device in slot %d:%d:%d\n", cfg->bus, cfg->dev, cfg->func);
		cmd |= PCI_COMMAND_MASTER;
		pci_write_config_word(cfg->bus, cfg->dev, cfg->func, PCI_COMMAND, cmd);
	}
	// Check the latency time, because certain BIOSes forget to set	//
	// it properly...						//
	lat = pci_read_config_byte(cfg->bus, cfg->dev, cfg->func, PCI_LATENCY_TIMER);
	if ( lat < 16 )
		lat = (64 <= pcibios_max_latency) ? 64 : pcibios_max_latency;
	else if ( lat > pcibios_max_latency )
		lat = pcibios_max_latency;
	else
		return;
	kprintf("PCI: Setting latency timer of device %d:%d:%d to %u\n", cfg->bus, cfg->dev, cfg->func, lat);
	pci_write_config_byte(cfg->bus, cfg->dev, cfg->func, PCI_LATENCY_TIMER, lat);
}
/* find the capability of a device
*/
int pci_find_capability(pci_cfg_t *cfg, int cap)
{
	unsigned short status;
	unsigned char pos, id;
	int ttl = 48;

	status = pci_read_config_word(cfg->bus, cfg->dev, cfg->func, PCI_STATUS);
	if ( !(status & PCI_STATUS_CAP_LIST) )
		return(0);

	switch (cfg->header_type & 0x7F)
	{
		case PCI_HEADER_TYPE_NORMAL:
		case PCI_HEADER_TYPE_BRIDGE:
		pos = pci_read_config_byte(cfg->bus, cfg->dev, cfg->func, PCI_CAPABILITY_LIST);
		break;

		case PCI_HEADER_TYPE_CARDBUS:
		pos = pci_read_config_byte(cfg->bus, cfg->dev, cfg->func, PCI_CB_CAPABILITY_LIST);
		break;

		default:
		return(0);
		break;
	}

	while (ttl-- && pos>=0x40)
	{
		pos &= ~3;
		id = pci_read_config_byte(cfg->bus, cfg->dev, cfg->func, pos+PCI_CAP_LIST_ID);
		if (id == 0xff)
			break;
		if (id == cap)
			return(pos);
		pos = pci_read_config_byte(cfg->bus, cfg->dev, cfg->func, pos+PCI_CAP_LIST_NEXT);
	}
	return(0);
}
/* sets the power state of a device
*/
int pci_set_power_state(pci_cfg_t *cfg, int state)
{
	int pm;
	unsigned short pmcsr;
	unsigned short pmc;

	// Bound the state to a valid range				//
	if (state > 3) state = 3;

	// Validate current state.					//
	// Can enter D0 from any state, but we can't go deeper if we're //
	// in a low power state.					//
	if (state > 0 && cfg->current_state > state)
		return(-1); //invalid einval
	else if (cfg->current_state == state)
		// we're already there 					//
		return(0);

	// find PCI PM capability in list 				//
	pm = pci_find_capability(cfg, PCI_CAP_ID_PM);

	// Abort if the device doesn't support PM capabilities		//
	if (!pm) return(-2); //error in IO 

	// Check if this device supports the desired state		//
	if (state == 1 || state == 2)
	{
		pmc = pci_read_config_word(cfg->bus, cfg->dev, cfg->func, pm+PCI_PM_PMC);
		if ( (state == 1 && !(pmc & PCI_PM_CAP_D1)) )
			return(-2); //error IO
		else if ( (state == 2 && !(pmc & PCI_PM_CAP_D2)) )
			return(-2); //error IO
	}

	// If we're in D3, force entire word to 0.			//
	// This doesn't affect PME_Status, disables PME_En, and		//
	// sets PowerState to 0.					//
	if ( cfg->current_state>=3 )
		pmcsr = 0;
	else
	{
		pmcsr = pci_read_config_word(cfg->bus, cfg->dev, cfg->func, pm+PCI_PM_CTRL);
		pmcsr &= ~PCI_PM_CTRL_STATE_MASK;
		pmcsr |= state;
	}

	// Enter specified state //
	pci_write_config_word(cfg->bus, cfg->dev, cfg->func, pm+PCI_PM_CTRL, pmcsr);

	// Mandatory power management transition delays			//
	// see PCI PM 1.1 5.6.1 table 18				//
	if( (state == 3) || (cfg->current_state == 3) )
	{
		// Set task state to interruptible			//
		// LINUX do it so:					//
		// 	set_current_state(TASK_UNINTERRUPTIBLE);	//
		// 	schedule_timeout(HZ/100);			//
		delay(100);
	}
	else if( (state == 2) || (cfg->current_state == 2) )
		// udelay(200);
		delay(200);
	cfg->current_state = state;

	return(0);
}
//enable device IO for athe device INTERNAL
int pcibios_enable_device_io(pci_cfg_t *cfg)
{
	unsigned short cmd, old_cmd;
	unsigned int i;

	kprintf("Low level enabling PCI device %d:%d:%d... \n", cfg->bus, cfg->dev, cfg->func);

	old_cmd = cmd = pci_read_config_word(cfg->bus, cfg->dev, cfg->func, PCI_COMMAND);
	for (i=0; i<sizeof(cfg->type); i++)
		if (cfg->type[i] == PCI_IO_RESOURCE_IO)
			// Command IO based				//
			cmd |= PCI_COMMAND_IO;

	if ( !(cmd & PCI_COMMAND_IO) )
	{
		// Device is not IO-based				//
		kprintf("Device is not IO-based!!!\n");
		return(-1); //invalid einval
	}

	if ( (cfg->header_type & 0x7F) == PCI_HEADER_TYPE_BRIDGE )
	{
		// Any PCI-to-PCI bridge must be enabled by setting	//
		// both I/O space and memory space access bits in the	//
		// command register.					//
		cmd |= PCI_COMMAND_MEMORY;
	}

	// Always enable bus master!!!					//
	cmd |= PCI_COMMAND_MASTER;

	if ( cmd!=old_cmd )
	{
		// Set the cache line and default latency (32)			//
		pci_write_config_word(cfg->bus, cfg->dev, cfg->func,
				PCI_CACHE_LINE_SIZE, (32 << 8) | (L1_CACHE_BYTES / sizeof(unsigned int)));
		// Enable the appropriate bits in the PCI command register	//
		pci_write_config_word(cfg->bus, cfg->dev, cfg->func, PCI_COMMAND, cmd);
		kprintf("OK!\n");
	}
	else
		kprintf("Already enabled.\n");
	return(0);
}
//enables a device for IO.
int pci_enable_device(pci_cfg_t *cfg)
{
	int err, pm;

	kprintf("Powering on PCI device %d:%d:%d... \n", cfg->bus, cfg->dev, cfg->func);
	pm = pci_set_power_state(cfg, 0);
	switch( pm )
	{
		case 0:
		kprintf("OK!\n");
		break;

		case (-2):
		kprintf("Device doesn't support Power Management Capabilities!!!\n");
		break;

		case (-1):
		kprintf("\n\rDevice is already in this power state.\n");
		break;
	}

	if ((err = pcibios_enable_device_io(cfg)) < 0)
		return(err);
	return(0);
}
//browse the PCI bus and find a device and enable it
int pci_find_cfg(pci_cfg_t *cfg, int enable)
{
	unsigned short bus, dev, func;
	pci_cfg_t *pcfg;

	pcfg = kmalloc( sizeof(pci_cfg_t) );

	for (bus=0; bus<4; bus++)
	for (dev=0; dev<32; dev++)
	for (func=0; func<8; func++)
	{
		if ( pci_probe(bus, dev, func, pcfg) )
		{
			if (
				cfg->base_class == pcfg->base_class &&
				cfg->sub_class == pcfg->sub_class &&
				cfg->interface == pcfg->interface
			)
			{
				// Device found				//
				memcpy(cfg, pcfg, sizeof(pci_cfg_t));
				// Enable the device if required	//
				if (enable) pci_enable_device(pcfg);
				// Free the temporary structure		//
				kfree(pcfg);
				return 1;
			}
		}
	}
	// Device not found						//
	kfree(pcfg);
	return 0;
}
void pci_dev_show_details(pci_cfg_t *cfg, bool d)
{
	int i;
	kprintf("PCI:%u:%u:%u", cfg->bus, cfg->dev,cfg->func);
	kprintf("Vendor :%04X Device :%04X SubSys_Vendor:%04X SubSys_Device:%04X\n",cfg->vendor_id, cfg->device_id, cfg->subsys_vendor, cfg->subsys_device);
	kprintf("Base_Class :%02X   Sub_Class  :%02X   Interface  :%02X\n",cfg->base_class, cfg->sub_class, cfg->interface);

	if(d)
	{	
		for (i=0; i<6; i++) // 6 BARs in PCI devices
		if (cfg->base[i])
		{
			if (cfg->type[i] == PCI_IO_RESOURCE_IO)
				kprintf("* BAR %d IO: %06x (%06x)\n",i, cfg->base[i], cfg->size[i]);
			else
	    			kprintf("* BAR %d MM: %010x (%010x)\n",i, cfg->base[i] & 0xfffffff0, cfg->size[i]);
		}
		if(cfg->rom_base)
			kprintf("* BAR ROM : %010x (%010x)\n",cfg->rom_base, cfg->rom_size);
	}	
	if (cfg->irq)
		kprintf("* Interrupt line: %u\n", cfg->irq);
	switch(cfg->header_type & 0x7F)
	{
		case PCI_HEADER_TYPE_NORMAL:
			kprintf("* Normal device\n");
			break;

		case PCI_HEADER_TYPE_BRIDGE:
			kprintf("* PCI <-> PCI bridge\n");
			break;

		default:
			kprintf("* Unknown header type\n");
			break;
	}
	kprintf("\n");
}
static int total_pcidevs;
//scan all pcidevice in the system and enable it
pci_cfg_t *pci_list;
void pci_scan()
{
	unsigned short bus, dev, func;
	unsigned int i;
	total_pcidevs = 0;
	pci_cfg_t pcfg,*end;
	if(!is_pci_present())
	{
		kprintf("No PCI BUS found\n");
		return;
	}
	pci_list = NULL;
	for (bus=0; bus<4; bus++)
	for (dev = 0; dev < 32; dev++)
	for (func = 0; func < 8; func++)
	{
		if ( pci_probe(bus, dev, func, &pcfg) )
		{
			kprintf("* Description : ");
			total_pcidevs++;
			for (i=0;; i++)
			{
				
				if ( i>=PCI_CLASS_ENTRIES )
				{
					kprintf("Unknown device!\n");
					break;
				}
				// show the human redable class name of device
				if(
					(classes[i].base_class == pcfg.base_class) &&
					(classes[i].sub_class == pcfg.sub_class) &&
					(classes[i].interface == pcfg.interface))
				{
					kprintf("%s\n", classes[i].name);
					break;
				}
			}
			pci_dev_show_details(&pcfg,0);
			pci_enable_device(&pcfg); // it doesn't harm if the device is already enabled.
			
			//here make a list of PCI devices so that we will refere it latter
			pci_cfg_t *temp = (pci_cfg_t *)kmalloc(sizeof(pci_cfg_t));
			if(!temp)
			{
				kprintf("Unable to allocate memory Halting\n");
				asm("cli");
				asm("hlt");
			}
			memcpy(temp,&pcfg,sizeof(pci_cfg_t));
			if(pci_list == NULL)
			{
				pci_list = temp;
				end = pci_list;
				pci_list->next=NULL;
				pci_list->prev=NULL;
			}
			else
			{
				temp->prev = end;
				end->next = temp;
				temp->next = NULL;
				end = temp;
			}
			kprintf("\n\r");
		}
	}
	kprintf("PCI: finished found %d PCI devices\n",total_pcidevs);
}
// browse the pci_list and return the device which is having class code and subclass code
pci_cfg_t *pci_get_dev(unsigned char class, unsigned char subclass)
{
	pci_cfg_t *pb,*pd=NULL;
	pb = pci_list;
	int total_dev =0;
	while(pb)
	{
		if((pb->base_class == class) && (pb->sub_class == subclass))
		{
			total_dev++;
			if(pd == NULL)			
				pd = pb;
			else
				pd -> next =pb;
			//return pd;
		}
		pb = pb->next;	
	}
	
	return pd;
}
unsigned int pci_dev_get_bar(pci_cfg_t *cfg, int bar_num)
{
	return (cfg->base[bar_num]);
}
struct pci_sig
	{
		unsigned char sig[4];
		void *entry;
		unsigned char rev_lev;
		unsigned char length;
		unsigned char cksum;
		unsigned char res[5];
	}__attribute__ ((packed));
// probing PCI BIOS
void probe_pci()
{
	unsigned char *rom_beg=(unsigned char *)0xe0000;
	unsigned char *rom_end=(unsigned char *)0xFFFFF;
	struct pci_sig *pci=(struct pci_sig *)rom_beg;
	do{
		
		if(strncmp((const char *)pci->sig,"_32_",4)==0)
		{
			int i;			
			for (i=0;i<4;i++)
			putch(pci->sig[i]);			
			kprintf("PCI BIOS found at %p\t",pci);
			kprintf("entry point at %p\n",pci->entry);
		}
		pci+=16;
	}while(pci<(struct pci_sig *)rom_end);
}

