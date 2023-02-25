#include "queue.h"
#include <stdlib.h>

void enqueue(int item, struct queue *q){
	struct node *p;
	p = (struct node *)malloc(sizeof(struct node));
	p->data = item;
	p->next = NULL;
	if (q->head == NULL)
	{
		q->head = q-> tail = p;
	}
	else
	{
		q->tail->next = p;
		q->tail = p;
	}
}

int dequeue(struct queue *q){
	int item;
	struct node *p;
	item = q->head->data;
	p = q->head;
	q->head = q->head->next;
	free(p);
	return item;
}