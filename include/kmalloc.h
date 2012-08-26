/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  Alloc defination                                       |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#ifndef __KMALLOC_H__
#define __KMALLOC_H__


void *kmalloc(unsigned int n);
void *krealloc(void *_r, unsigned int n);
void kfree(void *_r);
void __heap_trace(void);

#endif

