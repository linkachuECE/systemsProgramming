/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	03/21/23
 *      DESCRIPTION: 	The purpose of this header is to provide a data structure for keeping track of processes and process ID
 *      CONTRIBUTORS: 	None. I did not create this file, but downloaded it as is from D2L and added my own comments
 */

// Node structure, which can contain a PID and name of the process
struct node {
	int pid;
	char *name;
	struct node *next;
};

// Queue structure
struct queue {
	struct node *head;
	struct node *tail;
};

// Adds process to tail of queue
void enqueue(int item, char *s, struct queue *q) {
	struct node *p;						

	p = (struct node *)malloc(sizeof(struct node));
	p->pid = item;
	p->name = s;	
	p->next = NULL;
	if (q->head == NULL) q->head = q->tail = p;
	else {
		q->tail->next = p;
		q->tail = p;
	}
}

// Pops process from head of queue and returns it
int dequeue(struct queue *q) {
	int item;
	struct node *p;

	item = q->head->pid;
	p = q->head;
	q->head = q->head->next;
	free(p);
	return item;
}

// Delets a process from the queue
void delete(struct queue *q, int key) {
	if (q->head->pid == key){
		struct node *p = q->head;
		q->head = q->head->next;
		free(p);
		return;
	}

	struct node *current = q->head->next;
	struct node *previous = q->head;
	
	while (current != NULL && previous != NULL){
		if (current->pid == key){
			struct node *tmp = current;

			if (current == q->tail)
				q->tail = previous;

			previous->next = current->next;
			free(tmp);
			return;
		}

		previous = current;
		current = current->next;
	}
	return;  
}