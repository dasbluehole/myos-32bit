//queue
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
	
