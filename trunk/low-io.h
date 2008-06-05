/* Low level IO routines defined in low-io.asm
   These are C callable functions
*/
#ifndef _LOW_IO_H_
#define _LOW_IO_H_
//#include "gdt.h"
/*unsigned char inportb(unsigned short port); //reads a byte from port
unsigned short inportw(unsigned short port); // reads a word from a port
void outportb(unsigned short port,unsigned char value); //writes a byte to a port 
*/
inline static unsigned char inportb (unsigned short _port)
	{
		unsigned char rv;
		__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
		return rv;
	}

inline static void outportb (unsigned short _port, unsigned char _data)
	{
		__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
	}

inline static unsigned short inportw(unsigned short _port)
	{
		unsigned short rv;
		__asm__ __volatile__ ("inw %1, %0" : "=a" (rv) : "dN" (_port));
		return rv;
	}

inline static void outportw(unsigned short _port, unsigned short _data)
	{
		__asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
	}
inline static void enable()
{
	__asm__ __volatile__ ("sti": : );
}

inline static void disable()
{
	__asm__ __volatile__ ("cli" : :	);
}

inline static void halt()
{
	__asm__ __volatile__ ("hlt" : : );
} /*Halts cpu*/
//void gdt_flush(GDTR *x);
#endif

