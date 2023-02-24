#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>

int child_dead = 0;

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

void term_child(int sig_num){
	signal(SIGCHLD, term_child);
	child_dead = 1;
}


int main(int argc, char const *argv[])
{
	int qt, i, temp;
	struct queue q;
	struct node *p;

	if (argc>2)
	{
		qt = 1000*atoi(&argv[1][0]);
	}
	else
	{
		printf("%s qt prog1 [prog2] ... [prog[N}\n", argv[0]);
		exit(-1);
	}
	q.head = NULL;
	q.tail = NULL;
	signal(SIGCHLD, term_child);

	for (i = 2; i < argc; i++)
	{
		printf("Message from father: Creating program %s\n", argv[i]);
		if ((temp=fork())==0)
		{
			execl(argv[i],argv[i],NULL);
		}
		enqueue(temp,&q);
	}
	sleep(1);
	printf("\nI am the Scheduler and I will now begin scheduling my programs:\n");
	while(q.head!=NULL)
	{
		kill(q.head->data,SIGCONT);
		//usleep(qt);
		pause();
		if (child_dead==0)
		{
			kill(q.head->data,SIGUSR1);
			usleep(1000);
			enqueue(dequeue(&q),&q);
		}
		else
		{
			printf("A child is dead\n");
			dequeue(&q);
			child_dead=0;
		}
	}
	printf("All of my children died so I commit suicide! Bye...\n");
	return 0;
}
