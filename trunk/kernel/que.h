/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  Que Defination                                         |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#pragma once

#ifndef __QUE_H__
#define __QUE_H__

// the node structure
typedef struct node
{
	void *data;
	struct node *next, *prev;
}node_t;

//advance declaration
struct que_container;
//function pointer prototypes
typedef void (*q_fn)(struct que_container * c, void *dat);
typedef void *(*q_get)(struct que_container *q,int val);
typedef void (*flt)(node_t *n);
//que container
typedef struct que_container
{
	node_t *head; // from this point info for q is stored
	node_t *tail;
	node_t *cur;
	int 	num_nodes;
	q_fn	add;
	q_fn	del;
	q_get	retrive;
	q_get	get_next;
	q_fn	brwe;
	flt	filter;
	//q_fn	inie;
	char name[10];
}q_cont;
//function proto
q_cont *init_q(q_cont *q, void *name);
void add(q_cont *q, void *data);
void del(q_cont *q,void *data);
void browse(q_cont *q, void *x);
void filter(node_t *n);
void *retrive(q_cont *q,int val);
void *get_next(q_cont *q,int val);

#endif
