#ifndef __KMALLOC_H__
#define __KMALLOC_H__


void *kmalloc(unsigned int n);
void *krealloc(void *_r, unsigned int n);
void kfree(void *_r);
void __heap_trace(void);

#endif

