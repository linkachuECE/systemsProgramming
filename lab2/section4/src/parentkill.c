#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

/*
 *      AUTHOR: Ethan Braun
 *      DATE CREATED: 02/15/23
 *      DESCRIPTION: The purpose of this program is to demonstrate how to kill a process from its parent
 *      CONTRIBUTORS: None.
 */

// First process, which counts from the 0 to half the argument provided ad infinitum
void p1Func(int highest){
	for(int i = 0;; i++){
		printf("%d\n", i);
		if(i >= (highest >> 1)) i = 0;
	}
}

// Second process, which counts from the half the argument provided to the full number ad infinitum
void p2Func(int highest){
	for(int i = (highest >> 1);; i++){
		printf("%d\n", i);
		if(i > highest) i = highest >> 1;
	}
}


int main(int argc, char* argv[]){
	// Checking for proper usage
	if(argc != 2){
		printf("Usage: ./multiprocess NUM\n");
		exit(-1);
	}

	// Convert command line argument to an integer
	int finalVal = atoi(argv[1]);

	// Variables for keeping track of PIDs
	pid_t p1, p2;
	
	// First child
	if((p1 = fork()) < 0){
		perror("Creating first process");
		exit(-1);
	} else if (p1 == 0)
		p1Func(finalVal);
	
	// Second child
	if((p2 = fork()) < 0){
		perror("Creating second process");
		exit(-1);
	} else if (p2 == 0)
		p2Func(finalVal);

	// Tell parent process to wait for 5 seconds
	sleep(5);

	// Send a kill signal to both child processes
	kill(p1, SIGKILL);
	kill(p2, SIGKILL);
	
	exit(0);
}
