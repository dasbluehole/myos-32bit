//////////////////////////////////////////////////////////
// This file is a part of Nanos Copyright (C) 2008, 2009//
// ashok.s.das@gmail.com                                //
//////////////////////////////////////////////////////////
// TASK Implementation                                  //
// Taken from                                           //
// http://hosted.cjmovie.net/TutMultitask.htm           //
//////////////////////////////////////////////////////////
#include "kheap.h"
#include "stdio.h"
#include "string.h"
#include "idt.h"
#include "low-io.h"
#include "task.h"
#include "timer.h"
#include "que.h"



thread *curr;
static int current_task=-1;// no task is started.
//static int tid=1;  //tid=0 is for idle process
// this function will create a task and add to the threads[]
volatile int tasker=0;
extern thread *g_current;


q_cont *task_q;	// our task que 
q_cont *zombie;	// zombie que, all finished tasks will be here 	
q_cont *wait_q;	// wait que, all thread waiting for either IO/child


void thread_stack_push(thread *t, unsigned int val)
{
	t->stack_top-=4;
	*((unsigned int*)t->stack_top)=val;
}
// if a thread exits then it will be caught by this function 
static void thread_catcher()
{
	// do we have a parent?
	if(curr->pid>0)
	{

		// yes we have, so we see if it is waiting for us?
		//thread *parent=task_q->get_thread_by_id(curr->pid);
		thread *parent=(thread *)task_q->retrive(task_q,curr->pid);
		if(!parent) 
			goto no_parent;
		if(parent->state==WAITING)
		parent->state=RUNNING;
		parent->num_child--;
	}
	no_parent:
	// do we have some child who are active
	while(curr->num_child!=0)
	{	
		curr->state=WAITING;
	}
	curr->state=FINISHED;
	//curr->max_ticks=1;
	//cout<<"\n";
	for(;;);
}
void thread_run(func entry,unsigned int args)
{
	enable();	
	entry(args);
	thread_catcher();
}
static unsigned int new_tid()
{
	static unsigned int tid;
	tid++;
	return tid;
}
void wait_on_thread(thread *t)
{
	//if the child is not finished 
	if(t->state!=FINISHED)
	{
	// parent should wait
		curr->state=WAITING;
		curr->timeslice=curr->max_ticks;
	}
	sleep(10);
	//while(curr->num_child==0);
}

thread *create_thread(func entry,unsigned int args,PRIO p, bool detached)
{
	// Allocate a thread control structure	
	thread *t=(thread *)kmalloc(sizeof(thread));
	if(!t)
	{
		kprintf("Insufficient memory for Task creation\n");
		return NULL;
	} 
	memset((char*)t,'\0',sizeof(thread));
	t->stack_top=(unsigned int)t->stack+1024; // align the stack_top
	if((curr==NULL)|| detached)
		t->pid=0;          // either detached or first level thread 
	else
	{
		t->pid=curr->id;   // put the parent id
		curr->num_child++; // increase child count
	}	
	t->id=new_tid();			  // get the next tid
	t->timeslice=0;				  // No CPU time consumed
	t->p=p;					  // priority
	t->max_ticks=MAX_SLICE/p;		  // maximum eligible time 
						  // slice for the priority
	thread_stack_push(t,args);		  // push the address of 
						  // thread argument
	thread_stack_push(t,(unsigned int)entry); // push the thread 
						  // function
	thread_stack_push(t,(unsigned int)0);	  // push a fake return 
						  // address for thread func
	t->stack_top-=sizeof(struct regs);	  // allign thread context
						  // on the top of the stack
	t->r=(struct regs *)t->stack_top;		  // 
	t->r->gs = 0x10;			  // put the context
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
	t->r->eip=(unsigned int)thread_run;	  // thread_run will start
						  // our thread function
	//tt->err_code=0x;
	t->r->cs=0x8;
	t->r->eflags=0x0200;
	t->state=CREATED;				
	return (t);				  // return thread strucure
}
thread *get_next_runnable(q_cont *q)
{
	if(q==NULL)
	{
		kprintf("Error\n");
		disable();
		halt();
		for(;;);
	}
	else
	{
		thread *t;
		node_t *n = q->get_next(q,0);
		t=(thread*)n->data;
		while(1)
		{
			if(t->state==RUNNING)
				return t;
			n = q->get_next(q,0);
			t=(thread*)n->data;
		}
	}
}

void _print_state(states stat)
{
	switch(stat)
	{
		case INVALID: kprintf("INV\n");
				break;
		case CREATED: kprintf("CRE\n");
				break;
		case READY:   kprintf("RDY\n");
				break;
		case RUNNING: kprintf("RUN\n");
				break;
		case WAITING: kprintf("WAT\n");
				break;
		case BLOCKED: kprintf("BLC\n");
				break;
		case FINISHED: kprintf("FIN\n");
				break;
	}
}

void show_tasks(q_cont *q)
{
	int num_tasks=q->num_nodes;
	thread *th;
	node_t *n;
	int i;
	for(i=0;i<num_tasks;i++)
	{
		n = q->get_next(q,0);
		th=(thread*)n->data;
		kprintf("[%-2d] parent-id %-2d children %-2d ",th->id,th->pid,th->num_child);
		_print_state(th->state);
	}
}
void all_tasks()
{
	kprintf("The running...\n");
	show_tasks(task_q);
	kprintf("The Zombies...\n");
	show_tasks(zombie);
}

extern unsigned int read_eip();
//Switch between our two tasks
//Notice how we get the old esp from the ASM code
//It's not a pointer, but we actually get the ESP value
//That way we can save it in our task structure


unsigned int task_switch(void *sp)
{
	node_t *n;
	if(!tasker )			// our task que is not ready
		return(unsigned int)sp; // hence return what ever stack
	else
	{
					// we have a task_q and ...
		if(current_task==-1)	// have we started yet?
		{
			n = task_q->get_next(task_q,0);
			curr=(thread*)n->data;      // start the first task
			curr->state=RUNNING;
			current_task=0;          // mark we already started
			return(curr->stack_top); // return the current task
						 // note we dont save the 
						 // stack when we start the
						 // first task, is it right?
		}
		else			// we have already started
		{
			if(curr->state==CREATED) // first time task is here
				curr->state=RUNNING;
			if(curr->timeslice<curr->max_ticks) 
			{			   // our slice not finished
				curr->timeslice++; // we used one tick more 
				return ((unsigned int)sp); //return stack
			}
			else
				curr->stack_top=(unsigned int) sp; //well our slice
							   // finished
			n = task_q->get_next(task_q,0);
			curr = (thread *)n->data;	     // get next task
			if(curr->state==FINISHED) // is the task finished?
			{
				zombie->add(zombie,curr);    // put in deads				
				task_q->del(task_q,curr); // remove the task 
						      // from task que
				n = task_q->get_next(task_q,0);
				curr=(thread*)n->data;   // get next task as
						      // curr
			}
			if((curr->state==BLOCKED) || (curr->state==WAITING))
			{
				n = task_q->get_next(task_q,0);
				curr=(thread*)n->data;  // blocked or waiting
						     // get next
			}
			
			curr->timeslice=0;	     // mark slice unused
			return (curr->stack_top);    // return stack
		}
	}
}



void idle(unsigned int n)
{
	for(;;){
	//tm->sleep(10);
	//all_tasks();
	}
}
int a=10,b=20,c=30,d=40;
void thread1(unsigned int n)
{
	int x,y;
	for(;;)
	{	
		//disable();
		x=getX();
		y=getY();		
		gotoxy(70,5);
		putch('-');
		//kprintf("passed = %d\n",*(int*)n);
		//(*(int*)n)++;
		gotoxy(x,y);
		//enable();
		//sleep(10);
	}
}
void thread2(unsigned int n)
{
	int x,y;	
	for(;;)
	{
		//disable();
		x=getX();
		y=getY();		
		gotoxy(70,5);
		putch('\\');
		//kprintf("passed = %d\n",*(int*)n);
		gotoxy(x,y);
		//enable();
		//sleep(10);
	}
}
void thread3(unsigned int n)
{
	int x,y;	
	for(;;)
	{
		//disable();
		x=getX();
		y=getY();
		gotoxy(70,5);
		putch('|');
		//kprintf("passed = %d\n",*(int*)n);
		gotoxy(x,y);
		//enable();
		//sleep(10);
	}
}
void thread4(unsigned int n)
{
	int x,y;	
	for(;;)
	{
		//disable();
		x=getX();
		y=getY();		
		gotoxy(70,5);
		putch('/');
		//kprintf("passed = %d\n",*(int*)n);
		gotoxy(x,y);
		//enable();
		//sleep(10);
	}
}
/*void thread5(void *)
{
	shell *s=new shell();
	s->start();
}*/
void init_tasks()
{
	
	kprintf("initializing Tasks\n");
	task_q = init_q(task_q, "tasks");
	zombie = init_q(zombie, "zombie");
	//wait_q = init_q(wait_q, "Waiting");
	disable();
	// implement a locking mechanism while put() and get()
	thread *t=NULL;
	t= create_thread(idle,0,IDLE_PRIO,TRUE);
	if(t)
		task_q->add(task_q,t);
	t= create_thread(thread1,(unsigned int )&b,LOW_PRIO,TRUE);
	if(t)
		task_q->add(task_q,t);
	t= create_thread(thread2,(unsigned int )&c,LOW_PRIO,TRUE);
	if(t)
		task_q->add(task_q,t);
	t= create_thread(thread3,(unsigned int )&d,LOW_PRIO,TRUE);
	if(t)
		task_q->add(task_q,t);
	t= create_thread(thread4,0,HIGH_PRIO,TRUE);
	if(t)
		task_q->add(task_q,t);
	kprintf("Total %d tasks started\n",task_q->num_nodes);
	enable();
	tasker=1;
}


