#include "types.h"
#include "stdio.h"
#include "string.h"
#include "kheap.h"
#include "que.h"
#include "task.h"
void add(q_cont *q, void *data)
{
	// first we have to check if q is valid i.e not NULL
	if(q==NULL)
	{
		kprintf("The queue is not initialized, Please initialize it first\n");
		return;	
	}
	//printf("self = %0x\n",self);
	// OK q is initialized
	node_t *temp=(node_t *)kmalloc(sizeof(node_t));
	temp->data = data;
	// check if we are adding first element
	if(q->head == NULL)
	{
		// this is first element
		//printf("No elements %0x\n",temp);
		//temp->data = data;
		temp->prev = temp->next = NULL;
		q->cur = q->head = q->tail = temp;
	}
	else
	{
		// we have some element
		// tail gives us the address of last node so add there.
		//printf("next element %0x\n",temp);
		temp->prev = q->tail;
		temp->next = NULL;
		q->tail->next = temp;
		q->tail = q->tail->next;
	}
	q->num_nodes++;
}
void del(q_cont *q,void *data)
{
	//delete the node containing data from q
	// start from head and until tail is reached 
	// check if data is present in a node
	// if data is present then free that node and do housekeeping
	node_t *temp1, *temp2;
	if(!q)
	{
		kprintf("Que is not initialized\n");
		return;
	}
	temp1 = q->head;
	if(temp1->data == data) // the head node has the data
	{
		//printf("data at head\n");
		q->head = q->head->next;
		q->cur= q->head;
		kfree(temp1);
		q->num_nodes--;
		return;
	}
	//printf("data at other node\n");
	while(temp1 != NULL)
	{
		temp2 = temp1;
		//printf("Value at the node = %d\n",*(int*)temp1->data);
		if(temp1 ->next-> data == data)
		{
			//printf("found data\n");
			temp1 = temp1->next;
			temp2->next = temp1 -> next;
			kfree(temp1);
			q->num_nodes--;
			break;//temp1=temp2->next;
		}
		temp1=temp2->next;
	}
}
void browse(q_cont *q, void *x)
{
	if(!q)
	{
		kprintf("Que is not initialised\n");
		return;
	}
	/*node_t *temp = q->head;
	while(temp)
	{*/
		int i;
		for(i=0; i<q->num_nodes;i++)
		{
			kprintf("value = ");
			q->filter(q->get_next(q,0));
		}
		//temp = temp->next;
	//}
}
void fltr(node_t *n)
{
	kprintf("%d\n",*(int*)n->data);
}
void *retrive(q_cont *q,int val)
{
	node_t *temp;
	temp = q->head;
	while(temp)
	{
		if((((thread*)temp->data)->id) == val)
			return temp;
		temp = temp->next;	
	}
	return NULL;
}
void *get_next(q_cont *q,int val)
{
	node_t *temp;
	temp = q->cur;
	q->cur = q->cur->next;
	if(q->cur == NULL)
		q->cur = q->head;
	//q->cur = q->cur->next;
	return temp;
}
q_cont *init_q(q_cont *q, void *name)
{
	if(q)
	{
		kprintf("Queue is already initialized\n");
		return q;	
	}
	q = (q_cont*)kmalloc(sizeof(q_cont));
	if(!q)
	{
		kprintf("Insufficient Memory\n");
		return NULL;
	}
	//q->tq = q;
	q->head = NULL;
	q->tail = NULL;
	q->num_nodes = 0;
	q->add = add;
	q->del = del;
	q->brwe = browse;
	q->filter = fltr;
	q->retrive = retrive;
	q->get_next = get_next;
	strncpy(q->name,(char*)name,(strlen(name)>9 ? 9: strlen(name)));
	kprintf("Que initializition Done at %08x\n",q);
	return q;
}
