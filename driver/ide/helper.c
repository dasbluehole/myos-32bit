#include "types.h"
#include "low-io.h"
#include "timer.h"
#include "ide.h"
//==========================|helper functions|==============================
// depending on the second parameter sz
// reads a byte, word or dword from 
// port
unsigned int pio_inport( unsigned short port , size_t sz)
{
	switch(sz)
	{
		case 1: return inportb(port);
		case 2: return inportw(port);
		case 4: return inportl(port);
		default: return 0;
	}
}
// depending on the 3rd parameter sz
// sends a val byte/word/dword to
// port
void pio_outport(unsigned short port, unsigned int val, size_t sz)
{
	switch(sz)
	{
		case 1: outportb(port,(unsigned char)val); break;
		case 2: outportw(port,(unsigned short)val); break;
		case 4: outportl(port,(unsigned int)val); break;
	}
}
// reads single byte from port and returns it
unsigned char pio_inbyte(unsigned short port)
{
	return (unsigned char)pio_inport(port,1);
}
// reads a word 2 byts or short int
unsigned short pio_inword(unsigned short port)
{
	return (unsigned short)pio_inport(port,2);
}
// reads a dword(4-byte) from port and returns it
unsigned int pio_indword(unsigned short port)
{
	return (unsigned int)pio_inport(port,4);
}
// sends a byte value val to port
void pio_outbyte(unsigned short port,unsigned char val)
{
	pio_outport(port,val,1);
}
// sends a short value val to port
void pio_outword(unsigned short port,unsigned short val)
{
	pio_outport(port,val,2);
}
// sends a int(4-byte) val to port
void pio_outdword(unsigned short port,unsigned int val)
{
	pio_outport(port,val,4);
}
// reads count number of bytes from port to buffer
void pio_rep_inb(unsigned short port, unsigned char *buffer, unsigned int count)
{
	insb(port,buffer,count);
}
// reads count number of shorts from port to buffer
void pio_rep_inw(unsigned short port, unsigned short *buffer, unsigned int count)
{
	insw(port,buffer,count);
}
// reads count number of ints from port to buffer
void pio_rep_indw(unsigned short port, unsigned int *buffer, unsigned int count)
{
	insdw(port,buffer,count);
}
void pio_rep_outb(unsigned short port, unsigned char *buffer, unsigned int count)
{
	outsb(port, buffer, count);
}
void pio_rep_outw(unsigned short port, unsigned short *buffer, unsigned int count)
{
	outsw(port, buffer, count);
}
void pio_rep_outdw(unsigned short port, unsigned int *buffer, unsigned int count)
{
	outsdw(port, buffer, count);
}
unsigned char pio_get_status(unsigned short port)
{
	return pio_inbyte(port+STATUS_REG);
}
unsigned char pio_get_astatus(unsigned short port)
{
	return pio_inbyte(port+ALT_ST_REG);
}
bool pio_wait_busy(unsigned short port)
{
	// should have a delay here !!! guess it will work
	int i;
	for(i=0;i<4;i++)
		sleep(10); 
	return(pio_get_status(port)&STA_BSY);  // true if busy else false
}

bool pio_wait_busy_astat(unsigned short port)
{
	int i;
	for(i=0;i<4;i++)
		sleep(10); 
	return(pio_get_astatus(port)&STA_BSY);
}

// stop sending interrupts
// this should be called after selecting a drive 
void stop_ata_intr(unsigned short ctrl_port)
{
	pio_outbyte(ctrl_port,ATA_CTL_nIEN);
}

bool is_device_ready(slot *s)
{
	return(pio_inbyte(s->chanl->base_reg + ALT_ST_REG)&STA_DRDY);
}
bool is_device_busy(slot *s)
{
	return(pio_inbyte(s->chanl->base_reg + ALT_ST_REG)&STA_BSY);
}
