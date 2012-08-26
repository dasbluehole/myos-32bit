/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  Timer defination                                       |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
// timer header
#ifndef __TIMER_H__
#define __TIMER_H__


void install_timer();
int get_tick();
void sleep(int ms);

#endif

