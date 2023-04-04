#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/resource.h>

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	03/21/23
 *      DESCRIPTION: 	The purpose of this program is to serve as an executable for the shell simulation to launch as a child process
 *      CONTRIBUTORS: 	None.
 */

// Signal handler for continuing a process
void cont (int sig_num) {
     
}

// Signal handler for stopping the process momentarily
void stop (int sig_num) {
     pause();
}


int main(int argc, char *argv[]) {
	int i, num, sltime;

	// Establish signal handlers
	signal(SIGQUIT, cont);
	signal(SIGTSTP, stop);
	
	num = atoi(argv[1]);			// Number of times to print
	sltime = 1000*atoi(argv[2]);	// Time between each print

	// Hold the child process until the shell sends a signal to continue
	pause();
	
	// printing loop
	for (i = 1; i <= num; i++){
		printf("This is program %s and it prints for the %d time of %d...\n", argv[0], i, num);
		usleep(sltime);
	}
}
