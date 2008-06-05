#ifndef __BITOPS_H__
#define __BITOPS_H__
#include "low-io.h"
//set a bit 'nr' in the bit-map at 'addr'
__inline__ int set_bit(int nr,int * addr)
{
	int	mask, retval;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	disable();
	retval = (mask & *addr) != 0;
	*addr |= mask;
	enable();
	return retval;
}
//unset a bit 'nr' in the bit-map at 'addr'
__inline__ int clear_bit(int nr, int * addr)
{
	int	mask, retval;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	disable();
	retval = (mask & *addr) != 0;
	*addr &= ~mask;
	enable();
	return retval;
}
//test for a bit 'nr' is set or unset at 'addr'
__inline__ int test_bit(int nr, int * addr)
{
	int	mask;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *addr) != 0);
}
#endif
