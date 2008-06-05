#ifndef __IDT_H__
#define __IDT_H__ 
typedef struct IDT_entry
	{
		unsigned short base_low;
		unsigned short sel;
		unsigned char unused;
		unsigned char flags;
		unsigned short base_high;
	} __attribute__((packed)) IDT_entry;
typedef struct IDTR
	{
		unsigned short limit;
		unsigned int base;
	} __attribute__((packed)) IDTR;
typedef struct regs
	{	
		unsigned long gs, fs, es, ds;
		unsigned long edi, esi, ebp, esp, ebx, edx, ecx, eax;
		unsigned long int_no, err_code;
		unsigned long eip, cs, eflags, useresp, ss;	
	} regs;




typedef void (*isrfunc_t)(regs *);
void IDT_set_gate(unsigned char num, isrfunc_t func, unsigned short segment, unsigned char flags);
void set_handler(int intnum, isrfunc_t function);
void setup_IDT();

#endif

