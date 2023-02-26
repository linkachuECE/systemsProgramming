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
 * 						of a first-come, first-serve scheduler
 *      CONTRIBUTORS: 	None.
 */

int main(int argc, char const *argv[]){
	int i, temp;
	struct queue q;

	// Check if command line arguments follow proper format
	if(argc == 1){
		printf("%s prog1 [prog2] ... [prog[N]]\n", argv[0]);
		exit(-1);
	}

	// Initialize queue
	q.head = NULL;
	q.tail = NULL;

	// Execute all executables provided via the command line, one by one
	// Enqueue each process ID onto the queue
	for (i = 1; i < argc; i++){
		printf("Message from father: Creating program %s\n", argv[i]);
		if ((temp=fork())==0){
			execl(argv[i],argv[i],NULL);
		}
		enqueue(temp,&q);
	}

	sleep(1);
	printf("\nI am the FCFS Scheduler and I will now begin scheduling my programs:\n");
	// Run until the queue runs out of tasks to run
	while(q.head!=NULL){
		kill(q.head->data,SIGCONT);			// Continue process at the head of queue 1
		wait(&temp);						// Allow process to execute fully until terminated
		printf("A child is dead\n");
		dequeue(&q);						// Remove the process ID from the queue
	}

	printf("All of my children died so I commit suicide! Bye...\n");
	return 0;
}