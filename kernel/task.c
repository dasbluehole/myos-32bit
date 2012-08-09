//////////////////////////////////////////////////////////
// This file is a part of Nanos Copyright (C) 2008, 2009//
// ashok.s.das@gmail.com                                //
//////////////////////////////////////////////////////////
// TASK Implementation                                  //
// Taken from                                           //
// http://hosted.cjmovie.net/TutMultitask.htm           //
//////////////////////////////////////////////////////////
#include "kmalloc.h"
#include "vga.h"
#include "idt.h"
#include "irq.h"
#include "low-io.h"
#include "task.h"
#include "timer.h"
#include "queue.h"
using namespace IDT;
using namespace String;
using namespace IRQ;
typedef void (*func)(void *earg);
//thread threads[32]; //we will have maximum 32 tasks running.		
que<thread> *task_q=new que<thread>();
thread *curr;
static int current_task=-1;// no task is started.
static int tid=1;  //tid=0 is for idle process
// this function will create a task and add to the threads[]
volatile int tasker=0;
extern thread *g_current;
void thread_stack_push(thread *t, unsigned int val)
{
	t->stack_top-=4;
	*((unsigned int*)t->stack_top)=val;
}
static void thread_catcher()
{
	disable();
	cout.gotoxy(70,24);
	cout<<"exiting"	;
	curr->state=FINISHED;
	enable();
	for(;;);
}
void thread_run(func entry,void *args)
{
	cout<<"In thread_run\n";
	enable();	
	entry(args);
	disable();
	cout<<"Exit!!!\n";
	curr->state=FINISHED;
	enable();
}

thread *create_thread(func entry,void *args)
{
	disable();
	thread *t=new thread;
	if(!t)
	{
		cout<<"Insufficient memory for Task creation\n";
		enable();
		return NULL;
	}
	t->stack=new unsigned char[1024];
	if(t->stack==NULL)
	{
		cout<<"Insufficient memory for Task stack creation\n";
		delete (t);
		enable();
		return NULL;
	}
	t->stack_top=(unsigned int)t->stack+1024;
	t->arg=args;
	
	thread_stack_push(t,(unsigned int)args);
	thread_stack_push(t,(unsigned int)&thread_catcher);
	t->stack_top-=sizeof(IDT::regs);
	t->r=(IDT::regs *)t->stack_top;
	t->r->gs = 0x10;
	t->r->fs = 0x10;
	t->r->es = 0x10;
	t->r->ds = 0x10;
	t->r->edi=0;
	t->r->esi=0;
	t->r->ebp=0;
	t->r->esp=t->stack_top;
	t->r->ebx=0;
	t->r->edx=0;
	t->r->ecx=0;
	t->r->eax=0;
	t->r->eip=(unsigned int)entry;
	//tt->err_code=0x;
	t->r->cs=0x8;
	t->r->eflags=0x0200;
	t->state=CREATED;	
	enable();
	return (t);
}
extern "C" {extern unsigned int read_eip();};			
//Switch between our two tasks
//Notice how we get the old esp from the ASM code
//It's not a pointer, but we actually get the ESP value
//That way we can save it in our task structure
extern "C" {

void task_switch(void *sp)
{
	IDT::regs *r=(IDT::regs*)sp;
	if(current_task==-1)
	{
		disable();
		thread *temp=task_q->get(); // get the task
		current_task=0;
		memcpy(r,temp->r,sizeof(IDT::regs)); // switch regs
		task_q->put(temp); // put it back in q
		curr=temp;
		enable();
	}
	if(tasker)
	{
		thread *temp;
		memcpy(curr->r,r,sizeof(IDT::regs)); // save the register in 
                                                     // current task
		task_q->put(curr);                   // put the task in q
		curr=task_q->get();                  // get new task from q
		memcpy(r,curr->r,sizeof(IDT::regs)); // change rge registers
		enable();
	}
}

} //extern C
//TIMER *tm=TIMER::Instance();
void idle(void *)
{
	for(;;){
	//tm->sleep(10);
	}
}
int a=10,b=20,c=30,d=40;
void thread1(void *n)
{
	int x,y;
	for(;;)
	{	
		disable();
		x=cout.GetX();
		y=cout.GetY();		
		cout.gotoxy(70,5);
		cout<<"\\\n"<<"   "<<*(int*)n<<"   \n";
		(*(int*)n)++;
		cout.gotoxy(x,y);
		enable();
		//tm->sleep(10);
	}
}
void thread2(void *n)
{
	int x,y;	
	for(;;)
	{
		disable();
		x=cout.GetX();
		y=cout.GetY();		
		cout.gotoxy(70,5);
		cout<<"|\n"<<"   "<<*(int*)n<<"   \n";
		cout.gotoxy(x,y);
		enable();
		//tm->sleep(10);
	}
}
void thread3(void *n)
{
	int x,y;	
	for(;;)
	{
		disable();
		x=cout.GetX();
		y=cout.GetY();
		cout.gotoxy(70,5);
		cout<<"/\n"<<"   "<<*(int*)n<<"   \n";
		cout.gotoxy(x,y);
		enable();
		//tm->sleep(10);
	}
}
void thread4(void *n)
{
	int x,y;	
	//for(;;)
	{
		disable();
		x=cout.GetX();
		y=cout.GetY();		
		cout.gotoxy(70,5);
		cout<<"-\n"<<"   "<<*(int*)n<<"   \n";
		cout.gotoxy(x,y);
		enable();
		//tm->sleep(10);
	}
}
void thread5(void *)
{
	shell *s=new shell();
	s->start();
}
void init_tasks()
{
	cout<<"initializing Tasks\n";
	// implement a locking mechanism while put() and get()
	task_q->put(create_thread(idle,NULL));
	task_q->put(create_thread(thread1,(unsigned int *)&b));
	task_q->put(create_thread(thread2,(unsigned int *)&c));
	task_q->put(create_thread(thread3,(unsigned int *)&d));
	task_q->put(create_thread(thread4,(unsigned int *)&a));
	cout<<"Total "<<task_q->get_num_nodes()<<" tasks started\n";
	
	tasker=1;
}


