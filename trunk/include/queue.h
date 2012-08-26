/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  Que defination                                         |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
typedef struct node
{
	void *data;
	struct node *next,*prev;
}node;
node *head=NULL,*tail=NULL;
void add_at_head(void *data)
{
	node *anode;
	anode=node *kmalloc(sizeof(node));
	anode->data=data;	
	anode->next=head;
	anode->prev=NULL;
	if(head==NULL)
	
	head->prev=anode;
	head=anode;
}
void *get_from_tail()
{
	
