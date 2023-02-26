#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/resource.h>
#include <signal.h>

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED:	02/25/23
 *      DESCRIPTION: 	This program serves as a dummy program for
 * 						the scheduler to run several instances of
 *      CONTRIBUTORS: 	None.
 * 		OTHER DETAILS:	The functionality of this code is largely
 * 						unchanged from when it was downloaded from D2L
 */

/*
 * Function:  cont 
 * --------------------
 * 	Signal handler to continue the current process
 *
 * 	sig_num:	The signal received, which will be SIGCONT
 *
 *  returns: none
 */
void cont(int sig_num){
	signal(SIGCONT,cont);
	printf("Process %d starts...\n",getpid());
}

/*
 * Function:  stop 
 * --------------------
 * 	Signal handler to stop the current process
 *
 * 	sig_num:	The signal received, which will be SIGUSR1
 *
 *  returns: none
 */
void stop(int sig_num){
	signal(SIGUSR1,stop);
	printf("Process %d ends...\n",getpid());
	pause();
}


int main(int argc, char const *argv[])
{
	int i, num;
	num = atoi(&argv[0][1]);			// Retrieve number of times to print to the console
	signal(SIGUSR1,stop);				// Establish signal handler for stopping the process
	signal(SIGCONT,cont);				// Establish signal handler for continuing the process
	setpriority(PRIO_PROCESS,0,-20);	// Set the priority to -20
	pause();							// Pause until the parent process sends a signal to continue
	for (i = -5; i <= num; i++)
	{
		printf("This is program %s and it prints for the %d time...\n",argv[0],i+6 );
		usleep(10000);
	}
}