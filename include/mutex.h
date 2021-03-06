/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  Mutex Defination                                       |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#ifndef __MUTEX_H__
#define __MUTEX_H__

typedef volatile unsigned int lock_t;
extern void init_lock(lock_t *l);
extern int test_and_set(unsigned int val,lock_t *l);
typedef struct 
{
	lock_t lock;
}mutex_t;
void init_mutex(mutex_t *mutex); //defined in lock.s
void destroy_mutex(mutex_t *mutex);
void mutex_try_lock(mutex_t *mutex); // spin until lock
void mutex_unlock(mutex_t *mutex);   // unlock

#endif

