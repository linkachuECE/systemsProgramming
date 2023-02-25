#ifndef QUEUE_H
#define QUEUE_H
struct node
{
	int data;
	struct node *next;
};

struct queue
{
	struct node *head;
	struct node *tail;
};

#endif