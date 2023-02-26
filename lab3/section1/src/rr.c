#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>

#include "queue.h"

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED:	02/25/23
 *      DESCRIPTION: 	The purpose of this program is to simulate the functionality
 * 						of a round-robin scheduler
 *      CONTRIBUTORS: 	None.
 * 		OTHER DETAILS:	The functionality of this code is unchanged from
 * 						the code I downloaded from D2L.
 */

// Semaphore
int child_dead = 0;

// Signal handler for interrupt from child
// This increments the semaphore
void term_child(int sig_num){
	signal(SIGCHLD, term_child);
	child_dead = 1;
}

int main(int argc, char const *argv[]){
	int qt, i, temp;
	struct queue q;

	// Check if command line arguments follow proper format
	if (argc>2){
		qt = 1000*atoi(&argv[1][0]);	// Time quantum
	}else{
		printf("%s qt prog1 [prog2] ... [prog[N}\n", argv[0]);
		exit(-1);
	}

	// Initialize the queue
	q.head = NULL;
	q.tail = NULL;

	// Establish the child signal handler
	signal(SIGCHLD, term_child);

	// Execute all executables provided via the command line, one by one
	// Enqueue each process ID onto the queue
	for (i = 2; i < argc; i++){
		printf("Message from father: Creating program %s\n", argv[i]);
		if ((temp=fork())==0){
			execl(argv[i],argv[i],NULL);
		}
		enqueue(temp,&q);
	}

	sleep(1);
	printf("\nI am the Round Robin Scheduler and I will now begin scheduling my programs:\n");

	// Continue running while the queue contains elements
	while(q.head!=NULL){
		kill(q.head->data,SIGCONT);			// Continue process at the head of the queue
		usleep(qt);							// Allow process to run for the duration of the user-provided time quantum
		if (child_dead==0){					// Check the value of the semaphore
			kill(q.head->data,SIGUSR1);		// Signal child process to stop
			usleep(1000);
			enqueue(dequeue(&q),&q);		// Move current process to the tail of the queue
		}
		else{
			printf("A child is dead\n");
			dequeue(&q);					// Remove process from queue entirely, since it is finished
			child_dead=0;					// Decrement semaphore
		}
	}
	printf("All of my children died so I commit suicide! Bye...\n");
	return 0;
}
