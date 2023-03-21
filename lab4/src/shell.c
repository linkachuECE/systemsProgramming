#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <sys/resource.h>

#include "queue.h"

/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	03/21/23
 *      DESCRIPTION: 	The purpose of this program is to simulate the functionality of a shell, with multiple possible scheduling policies
 *      CONTRIBUTORS: 	None.
 */


// Macros for scheduling policies
#define FCFS 1
#define ROUNDROBIN 2
#define MFQ 3

// Globals
int schedPol = FCFS;	// Scheduling policy
int fg_pid = 0;			// Foreground PID
int fg_suspended = 0;	// Keeps track if foreground PID is suspended
int run = 1;			// Ends shell if set to 0
struct queue pid_list;	// List of PID's for ps command
int child_dead = 0;		// Set to 1 if a child process has been killed or terminated

// Round Robin and MFQ time quantums
#define INITTQ 1000
int tq[] = {INITTQ, INITTQ, INITTQ, INITTQ, INITTQ, INITTQ, INITTQ, INITTQ, INITTQ, INITTQ};
int queueNum = 2;	// Number of queues for MFQ

// Manual page
void help() {
	printf("This is manual page\n");
	printf("This shell supports the following commands:\n");
	printf("\tver\n\texec\n\tps\n\tkill\n\thelp\n\texit\n");
	printf("For more details please type 'help command'\n");
}

// Displays manual for individual commands
void helpcmd(char *cmd) {
	printf("This is manual page\n\n");
	if (strcmp(cmd, "ver") == 0) {
		printf("\nver:\tShows details about the shell version\n");
	} else if (strcmp(cmd, "exec") == 0) {
		printf("\nexec p1(n1,qt1) p2(n2,qt2) ...:\nExecutes the programs p1, p2 ...\nEach program types a message for n times and it is given a time quantum of qt msec.\n");
		printf("If parameter (&) is given the program will be executed in the background\n");
	} else if (strcmp(cmd, "ps") == 0) {
		printf("\nps:\tShows the living process with the given pid\n");
	} else if (strcmp(cmd, "kill") == 0) {
		printf("\nkill pid:\tEnds the process with the given pid\n");
	} else if (strcmp(cmd, "help") == 0) {
		printf("\nhelp:\tYou should know this command by now\n");
	} else if (strcmp(cmd, "exit") == 0) {
		printf("\nexit:\tEnds the experience of working in the new shell\n");
	} else if (strcmp(cmd, "set_scheduling") == 0) {
		printf("\nset_scheduling:\tSets the scheduling policy to one of three values: FCFS, Round Robin, or MFQ\n");
	} else {
		printf("\nNo Such command. Type help to see a list of commands\n");
	}
}

// Verifies that the shell is working
void ver() {
	printf("\nNew Shell. Works properly!!\n");
}

// Displays list of child process names and PIDs
void ps() {
	struct node *p;
	printf("\nNEW SHELL presents the following living processes\n");
	printf("\tPID\tNAME\n");
	for (p = pid_list.head; p != NULL; p = p->next){
		printf("\t%d\t%s\n",p->pid,p->name);
	}
}

// Kills a child process
void mykill(int pid) {
	kill(pid, SIGTERM);
	printf("You have just killed process %d\n", pid);
}

// Executes a single program and loads the PID onto the PID queue
int exec(char *input) {
	int i,t;
	char *args[10];
	char *temp;

	for (i = 0; i < 10; i++){
		args[i]=(char *)malloc(10*sizeof(char));
	}

	strcpy(args[0], strtok(input, "(,"));
	for (i = 1; (temp = strtok(NULL, ",)")) != NULL; i++) 
		strcpy(args[i],temp);
	printf("\n");
	if (strcmp(args[i-1], "&") == 0){
		args[i-1]=NULL;
	} else
		args[i]=NULL;
	if ((t = fork()) == 0){
		execv(args[0], args);
	}
	enqueue(t,args[0], &pid_list);

	return t;
}

// Execute processes in a first-come first-serve queue
void execFCFS(char programs[15][30], int num){
	for(int i = 0; i < num; i++){
		int pd = exec(programs[i]);
		usleep(500);
		kill(pd, SIGQUIT);
		pause();
		child_dead = 0;
	}
}

// Execute processes with a round robin scheduling policy
void execRoundRobin(char programs[15][30], int num){
	struct queue rr;
	rr.head = NULL;

	for(int i = 0; i < num; i++){
		int pid = exec(programs[i]);
		usleep(500);
		enqueue(pid, "p", &rr);
	}

	printf("Executing Round Robin scheduling with a time quantum of %d\n", *tq);
	while(rr.head){
		printf("Continuing child process %d\n", rr.head->pid);
		kill(rr.head->pid, SIGQUIT);
		usleep(*tq);
		if(child_dead){
			printf("A child is dead\n");
			dequeue(&rr);
			child_dead = 0;
		} else {
			printf("Stopping child process %d\n", rr.head->pid);
			kill(rr.head->pid, SIGTSTP);
			usleep(1000);
			enqueue(dequeue(&rr), "p", &rr);
		}
	}
}

// Execute process with a multi-feedback queue scheduling policy
void execMFQ(char programs[15][30], int num){
	struct queue queueList[queueNum];
	for(int i = 0; i < queueNum; i++){
		queueList[i].head = NULL;
		queueList[i].tail = NULL;
	}

	for(int i = 0; i < num; i++){
		int pid = exec(programs[i]);
		usleep(500);
		enqueue(pid, "p", &queueList[0]);
	}

	printf("Executing MFQ scheduling\n");

	for(int i = 0; i < queueNum;){
		int nextQueue = (i == (queueNum - 1)) ? 0 : i + 1;
		if(!queueList[i].head && !queueList[nextQueue].head){
			printf("\n");
			break;
		}

		printf("Running processes in queue %d in %dms timeslices\n", i+1, tq[i] / 1000);
		while(queueList[i].head){
			printf("Continuing child process %d\n", queueList[i].head->pid);
			kill(queueList[i].head->pid, SIGQUIT);
			usleep(tq[i]);
			if(child_dead){
				printf("A child is dead\n");
				dequeue(&queueList[i]);
				child_dead = 0;
			} else {
				printf("Stopping child process %d\n", queueList[i].head->pid);
				kill(queueList[i].head->pid, SIGTSTP);
				usleep(1000);
				enqueue(dequeue(&queueList[i]), "p", &queueList[nextQueue]);
			}
		}
		printf("\n");
		i = nextQueue;
	}
}

// Directs to one of the three scheduling policies based on the configuration
void execAll(char programs[15][30], int num){
	if(schedPol == FCFS)
		execFCFS(programs, num);
	if(schedPol == ROUNDROBIN)
		execRoundRobin(programs, num);
	if(schedPol == MFQ)
		execMFQ(programs, num);
}

// Exit the shell
void myexit() {
	char yesno;
	if (pid_list.head == pid_list.tail){
		run = 0;
	} else {
		printf("There are still living processes Do you want to kill them? (y/n): ");
		yesno=getchar();
		if (yesno == 'y'){
			while(pid_list.head != pid_list.tail){
				mykill(pid_list.tail->pid);
				usleep(10000);
			}
			run=0;
		}
	}
}

// Set the scheduling policy
void set_scheduling(char* input){
	if(strcmp(input, "FCFS") == 0){
		schedPol = FCFS;
		printf("Scheduling policy changed to First-Come, First-Served\n");
	} else if(strcmp(input, "Round") == 0){
		schedPol = ROUNDROBIN;
		printf("Please enter a time quantum (in ms): ");
		scanf("%d", tq);
		*tq *= 1000;

		printf("Scheduling policy changed to Round Robin\n");
	} else if(strcmp(input, "MFQ") == 0){
		schedPol = MFQ;

		do {
			printf("Please enter the number of queues you'd like to use: ");
			scanf("%d", &queueNum);
			if(queueNum > 10)
				printf("Please provide a number between 1 and 10\n");
		} while (queueNum > 10 || queueNum < 1);

		for(int i = 0; i < queueNum; i++){
			printf("Please enter a time quantum (in ms) for queue %d: ", i + 1);
			scanf("%d", tq + i);
			tq[i] *= 1000;
		}

		printf("Scheduling policy changed to Multi-feedback Queue\n");
	} else {
		printf("Not a valid scheduling policy\n");
	}
}

// Signal handler for when a child process dies
void childdead(int signum) {
	int dead_pid, status;
	
	dead_pid = wait(&status);
	printf("The child %d is dead\n", dead_pid);

	delete(&pid_list,dead_pid);
	printf("\n");
	if (dead_pid == fg_pid){
		fg_pid=0;
	}

	child_dead = 1;
}

// Signal handler for when the shell is stopped
void susp(int signum) {
	fg_suspended = 1;
	printf("All processes supspended\n");
}

// Signal handler for when the shell is continued
void cont(int signum) {
	fg_suspended = 0;
	printf("Waking all processes...\n");
	while (fg_pid != 0 && fg_suspended != 1)
		pause();
}

int main(int argc, char* argv[]) {
	char input[15][30];
	int argnum;

	pid_list.head=NULL;
	pid_list.tail=NULL;
	enqueue(getppid(), "NEW SHELL", &pid_list);

	signal(SIGCHLD, childdead);
	signal(SIGTSTP, susp);
	signal(SIGQUIT, cont);
	setpriority(PRIO_PROCESS, 0, -20);

	ver();

	while (run){
		printf("=>");
		for (argnum=0; (scanf("%s",input[argnum]))==1;argnum++)
			if (getchar() == '\n') break;
		if (strcmp(input[0],"ver") == 0 && argnum==0) ver();
		else if (strcmp(input[0],"help") == 0 && argnum == 0) help();
		else if (strcmp(input[0],"help") == 0 && argnum == 1) helpcmd(input[argnum]);
		else if (strcmp(input[0],"ps") == 0 && argnum == 0) ps();
		else if (strcmp(input[0],"kill") == 0 && argnum == 1) mykill(atoi(input[1]));
		else if (strcmp(input[0],"exec") == 0 && argnum != 0) execAll(input + 1, argnum);
		else if (strcmp(input[0],"exit") == 0 && argnum == 0) myexit();
		else if (strcmp(input[0],"set_scheduling") == 0 && argnum > 0) set_scheduling(input[1]);
	    else printf("No such command. Check help for help.\n");
	}

}
