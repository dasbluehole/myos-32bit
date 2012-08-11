//////////////////////////////////////////////////////////
// This file is a part of Nanos Copyright (C) 2008, 2009//
// ashok.s.das@gmail.com                                //
//////////////////////////////////////////////////////////
// TASK Defination                                  //
// Taken from                                           //
// http://hosted.cjmovie.net/TutMultitask.htm           //
//////////////////////////////////////////////////////////
#ifndef __TASK_H__
#define __TASK_H__

#include "idt.h"
#include "low-io.h"
#include "types.h"
typedef enum states{
		INVALID=0,
		CREATED=1,
		READY,
		RUNNING,
		WAITING,
		BLOCKED,
		FINISHED,
	}states;
typedef enum priority{
		HIGH_PRIO=1,
		NORM_PRIO=2,
		LOW_PRIO=4,
		IDLE_PRIO=100,
}PRIO;
#define MAX_SLICE 200 		  // 200 ticks 200 milisecs perhaps... 
                      		  // as weare generating 1000 pulses
typedef struct thread
{
	unsigned int stack_top;   // top of the stack goes to esp
	unsigned char stack[1024];// a static stack for the thread
				  // bad idea but it helps us when we 
				  // remove the thread
	//char name[20]; 		  // thread name 19 chars + \0	
	struct regs *r;  		  // thread context
	// void *arg;		  // hopefully we don't need this
	unsigned int timeslice;   //to keep record of how much time remains
	PRIO p;                   // priority
	unsigned int max_ticks;   // how much time it will have in every 
				  // time it gets the CPU
	states state;		  // state RUNNING,WAITING ... etc...
	int retval;		  // return value of the thread.
	unsigned int id;	  // threads id
	unsigned int pid;	  // parent id if it has one
	int num_child; 		  // number of child threads of this thread
}thread;

typedef void (*func)(unsigned int earg); // prototype for thread function

thread *create_thread(func entry,unsigned int args,PRIO p, bool detached);
unsigned int task_switch(void *sp);
extern thread *g_current;
void init_tasks();
void all_tasks();

#endif
