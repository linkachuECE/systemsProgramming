#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED:	02/25/23
 *      DESCRIPTION: 	The purpose of this program is to launch a child process which executes a program provided 
 *                      by the command line, and wait a specified amount of time before terminating the child process
 *      CONTRIBUTORS: 	None.
 */

// Variable to hold the child PID
int cpid;

// Signal handler for an alarm signal
void alarmHandler(int sig){
    printf("Parent: Alarm signal received\n");

    // Send a kill signal to the child process
    if(kill(cpid, SIGKILL) == -1)
        perror("Sending kill signal to child");
    else
        printf("Killed child process %d successfully\n", cpid);
    exit(0);
}

// Signal handler for a child signal
void childHandler(int sig){
    printf("Child %d terminated\n", cpid);
    exit(0);
}

int main(int argc, char* argv[]){
    int secs, temp;

    // Check that the command line arguments are in the proper format
    if(argc != 3){
        printf("Usage: %s parentwaittime prog\n", argv[0]);
        exit(-1);
    }

    // Fork a child process and execute the provided
    if((cpid = fork()) < 0){
        perror("Creating child");
    } else if(cpid == 0){
        if (execlp(argv[2], argv[2], (char*)NULL) == -1){
            perror("Executing provided program");
            exit(-1);
        }
    }

    // Establish signal handlers for child and alarm
    signal(SIGCHLD, childHandler);
    signal(SIGALRM, alarmHandler);

    // Get seconds from command line
    secs = atoi(argv[1]);

    // Set alarm
    alarm(secs);
    // Wait for child to finish
    waitpid(cpid, &temp, 0);

    exit(0);
}