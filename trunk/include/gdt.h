#ifndef __GDT_H__
#define __GDT_H__
/* Access byte's flags */
#define ACS_PRESENT     0x80            /* present segment */
#define ACS_CSEG        0x18            /* code segment */
#define ACS_DSEG        0x10            /* data segment */
#define ACS_CONFORM     0x04            /* conforming segment */
#define ACS_READ        0x02            /* readable segment */
#define ACS_WRITE       0x02            /* writable segment */
#define ACS_IDT         ACS_DSEG        /* segment type is the same type */
#define ACS_INT_GATE    0x0E            /* int gate for 386 */
#define ACS_INT         (ACS_PRESENT | ACS_INT_GATE) /* present int gate */

/* Ready-made values */
#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
/* Segment desciptor definition */
typedef struct DESCR_SEG 
{
	unsigned short limit,
		       base_l;
	unsigned char  base_m,
       		       access,
                       attribs,
                       base_h;
} __attribute__ ((packed)) DESCR_SEG;

/* GDTR register definition */
typedef struct GDTR{
  		unsigned short limit;
  		unsigned long  base;
} __attribute__ ((packed)) GDTR;

/* Interrupt desciptor definition */
typedef struct DESCR_INT{
		unsigned short offset_l,
			       selector;
		unsigned char  param_cnt,
       			       access;
  		unsigned short offset_h;
} __attribute__ ((packed)) DESCR_INT;

/* IDTR register definition 
typedef struct IDTR{
		unsigned short limit;
  		unsigned int base;
} __attribute__ ((packed)) IDTR;   */

void lgdt(GDTR *gdtr);

void setup_GDT_entry (DESCR_SEG *item, unsigned int base, unsigned int limit, unsigned char access, unsigned char attribs);
void setup_GDT();


#endif
