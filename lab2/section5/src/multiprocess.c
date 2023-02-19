#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

#define MESSAGESIZE 128

/*
 *      AUTHOR: Ethan Braun
 *      DATE CREATED: 02/15/23
 *      DESCRIPTION: The purpose of this program is to demonstrate pipes, signals, and other forms of interprocess communication
 *      CONTRIBUTORS: None.
 */

// Child 0 function
void c0Func(int* pToC0, int* c0ToP){
	// Holds child 1 PID
	pid_t c1;

	// Close unnecessary ends of pipes
	close(pToC0[1]);
	close(c0ToP[0]);

	// Read Child 1 PID from parent process and display
	printf("Child 0: Waiting for Child 1 PID from parent...\n");
	read(pToC0[0], &c1, sizeof(c1));
	printf("Child 0: Received Child 1 PID: %d\n", c1);

	// Send a termination signal to child 1
	printf("Child 0: Sending Termination signal to child 1\n");
	kill(c1, SIGTERM);

	// Send a message to the parent indicating that child 1 has been terminated
	printf("Child 0: Sending message to parent\n");
	char* message = "Terminated process 1";
	write(c0ToP[1], message, strlen(message) * sizeof(char));

	// Close pipes and exits
	printf("Child 0: Exiting...\n");
	close(pToC0[0]);
	close(c0ToP[1]);
	exit(0);
}

// Function to terminate child 1
void c1Terminate(int sig){
	printf("Child 1: Received termination signal.\n");
	printf("Child 1: Exiting...\n");
	exit(0);
}

// Child 1 function
void c1Func(){
	// Handle signals properly, then run an infinite loop
	printf("Child 1: Running infinite loop\n");
	signal(SIGTERM, c1Terminate);
	for(;;);
}

int main(int argc, char* argv[]){
	
	// pipe[1] -> write
	// pipe[0] -> read

	pid_t c0, c1;		// Children PIDs
	int pToC0[2];		// Pipe to send information from parent to child 0
	int c0ToP[2];		// Pipe to send information from child 0 to parent

	// Initialize pipes
	pipe(pToC0);
	pipe(c0ToP);

	// Start child 0
	printf("Parent: Starting child 0\n");
	if((c0 = fork()) < 0){
		perror("Creating child 0");
		exit(-1);
	} else if(c0 == 0){
		c0Func(pToC0, c0ToP);
	}

	// Child process 1
	printf("Parent: Starting child 1\n");
	if((c1 = fork()) < 0){
		perror("Creating child 1");
		exit(-1);
	} else if (c1 == 0){
		c1Func();
	}

	// Hold parent process for 3 seconds
	printf("Parent: Sleeping for three seconds...\n");
	sleep(3);

	// Close unneccesary pipe end and send Child 1 ID to Child 0
	printf("Parent: Sending Child 1 PID to Child 0\n");
	close(pToC0[0]);
	write(pToC0[1], &c1, sizeof(c1));

	// Create a buffer to store message from Child 0
	char messageBuf[MESSAGESIZE];

	// Close unneccesary pipe end and receive message from Child 0
	printf("Parent: Receiving message from Child 0\n");
	close(c0ToP[1]);
	read(c0ToP[0], messageBuf, MESSAGESIZE);

	// Wait for children to finish
	int temp;
	wait(&temp);

	// Print received message to terminal
	printf("Parent: Received message from child 0: \"%s\"\n", messageBuf);

	// Close all pipes
	close(c0ToP[0]);
	close(pToC0[1]);

	printf("Parent: Exiting...\n");
	exit(0);
}
