#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>

#include "queue.h"

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	02/25/23
 *      DESCRIPTION: 	The purpose of this program is to simulate the functionality
 * 						of a multi-level feedback queue scheduler with 2 round-robin queues
 *      CONTRIBUTORS: 	None.
 */

// Semaphore
int child_dead = 0;

// Signal handler for interrupt from child
void term_child(int sig_num){
	signal(SIGCHLD, term_child);
	child_dead = 1;
}

int main(int argc, char const *argv[]){
	int qt1, qt2, i, temp;
	struct queue q1, q2;

	// Check if command line arguments follow proper format
	if (argc>3) {
		qt1 = 1000*atoi(&argv[1][0]);	// Time quantum for queue 1
		qt2 = 1000*atoi(&argv[2][0]);	// Time quantum for queue 2
	} else {
		printf("%s qt1 qt2 prog1 [prog2] ... [prog[N]]\n", argv[0]);
		exit(-1);
	}

	// Initialize both queues
	q1.head = NULL;
	q1.tail = NULL;

	q2.head = NULL;
	q2.tail = NULL;

	// Establish the child signal handler
	signal(SIGCHLD, term_child);

	// Execute all executables provided via the command line, one by one
	// Enqueue each process ID onto queue 1
	for (i = 3; i < argc; i++){
		printf("Message from father: Creating program %s\n", argv[i]);
		if ((temp=fork())==0){
			execl(argv[i],argv[i],NULL);
		}
		enqueue(temp,&q1);
	}

	sleep(1);
	printf("\nI am the MFQ Scheduler and I will now begin scheduling my programs:\n");

	// Continue running while at least one queue contains elements
	while(q1.head!=NULL || q2.head!=NULL){
		printf("Running processes in queue 1 in %dms timeslices\n", qt1 / 1000);
		while(q1.head!=NULL){
			kill(q1.head->data,SIGCONT);		// Continue process at the head of queue 1
			usleep(qt1);						// Allow process to run for the duration of the user-provided time quantum qt1
			if (child_dead==0) {				// Check the value of the semaphore
				kill(q1.head->data,SIGUSR1);	// Signal child process to stop
				usleep(1000);
				enqueue(dequeue(&q1),&q2);		// Move current process to tail of queue 2
			} else {
				printf("A child is dead\n");
				dequeue(&q1);					// Remove process from queue entirely, since it is finished
				child_dead=0;					// Decrement semaphore
			}
		}
		// Break if both queues are empty
		if(q1.head==NULL && q2.head==NULL){
			printf("\n");
			break;
		}
		printf("Running processes in queue 2 in %dms timeslices\n", qt2 / 1000);
		while(q2.head!=NULL){
			kill(q2.head->data,SIGCONT);		// Continue process at the head of queue 2
			usleep(qt2);						// Allow process to run for the duration of the user-provided time quantum qt1
			if (child_dead==0) {				// Check the value of the semaphore
				kill(q2.head->data,SIGUSR1);	// Signal process to stop
				usleep(1000);
				enqueue(dequeue(&q2),&q1);		// Move current process to tail of queue 1
			} else {
				printf("A child is dead\n");	
				dequeue(&q2);					// Remove process from queue entirely, since it is finished
				child_dead=0;					// Decrement semaphore
			}
		}
		printf("\n");
	}
	printf("All of my children died so I commit suicide! Bye...\n");
	return 0;
}
