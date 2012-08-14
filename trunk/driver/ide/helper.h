#ifndef __HELPER_H__
#define __HELPER_H__
#include "types.h"
unsigned char pio_inbyte(unsigned short port);
unsigned short pio_inword(unsigned short port);
unsigned int pio_indword(unsigned short port);
void pio_outbyte(unsigned short port,unsigned char val);
void pio_outword(unsigned short port,unsigned short val);

void pio_outdword(unsigned short port,unsigned int val);
void pio_rep_inb(unsigned short port, unsigned char *buffer, unsigned int count);
void pio_rep_inw(unsigned short port, unsigned short *buffer, unsigned int count);
void pio_rep_indw(unsigned short port, unsigned int *buffer, unsigned int count);
void pio_rep_outb(unsigned short port, unsigned char *buffer, unsigned int count);
void pio_rep_outw(unsigned short port, unsigned short *buffer, unsigned int count);

void pio_rep_outdw(unsigned short port, unsigned int *buffer, unsigned int count);
unsigned char pio_get_status(unsigned short port);
unsigned char pio_get_astatus(unsigned short port);
bool pio_wait_busy(unsigned short port);
bool pio_wait_busy_astat(unsigned short port);
bool is_device_ready(slot *s);
bool is_device_busy(slot *s);

#endif

