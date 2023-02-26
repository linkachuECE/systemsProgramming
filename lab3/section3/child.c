#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED:	02/25/23
 *      DESCRIPTION: 	This program serves as a supplementary program to be launched from the main program timeout
 *                      It only runs for 5 seconds.
 *      CONTRIBUTORS: 	None.
 */

// Signal handler for kill signal
void killHandle(int sig){
    printf("Child: received kill signal\n");
    exit(0);
}


int main(int argc, char* argv[]){
    // Establish kill signal handler
    signal(SIGKILL, killHandle);

    sleep(5);
    exit(0);
}