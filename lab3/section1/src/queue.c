#include "queue.h"
#include <stdlib.h>

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED:	02/25/23
 *      DESCRIPTION: 	The purpose of the queue functions is to provide a 
 * 						mechanism for the scheduler to schedule processes
 *      CONTRIBUTORS: 	None.
 */

/*
 * Function:  enqueue 
 * --------------------
 * 	Adds an item onto the tail of the provided queue
 *
 *  item: 	Element to be added to the tail of the queue
 * 	q:		The provided queue
 *
 *  returns: none
 */
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

/*
 * Function:  dequeue 
 * --------------------
 * 	Removes an element from the head of the queue and returns its data
 *
 * 	q:			queue to remove from
 *
 *  returns: 	data in head of queue
 */
int dequeue(struct queue *q){
	int item;
	struct node *p;
	item = q->head->data;
	p = q->head;
	q->head = q->head->next;
	free(p);
	return item;
}