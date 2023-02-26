#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <string.h>

#include "queue.h"

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED:	02/25/23
 *      DESCRIPTION: 	The purpose of this program is to simulate the functionality
 * 						of a shortest-job-first scheduler
 *      CONTRIBUTORS: 	None.
 */

int main(int argc, char const *argv[]){
	int i, temp, pNo;
	struct queue q;
	pNo = argc - 1;

	char processes[pNo][10];

	// Check if command line arguments follow proper format
	if(argc == 1){
		printf("%s prog1 [prog2] ... [prog[N]]\n", argv[0]);
		exit(-1);
	}

	// Move processes in argument list to a new array
	for(int i = 0; i < pNo; i++){
		strcpy(processes[i], argv[i + 1]);
	}

	// Sort the new array
	qsort(processes, pNo, sizeof(processes[0]), strcmp);

	// Initialize the queue
	q.head = NULL;
	q.tail = NULL;

	// Execute all executables provided via the command line, one by one
	// Enqueue each process ID onto the queue
	for (i = 0; i < pNo; i++){
		printf("Message from father: Creating program %s\n", processes[i]);
		if ((temp=fork())==0){
			execl(processes[i],processes[i],NULL);
		}
		enqueue(temp,&q);
	}

	sleep(1);
	printf("\nI am the Shortest-Job-First Scheduler and I will now begin scheduling my programs:\n");
	
	// Continue running while the queue contains elements
	while(q.head!=NULL){
		kill(q.head->data,SIGCONT);		// Signal child process to stop
		wait(&temp);					// Allow process to execute fully until terminated
		printf("A child is dead\n");	
		dequeue(&q);					// Remove the process ID from the queue
	}
	printf("All of my children died so I commit suicide! Bye...\n");
	return 0;
}
