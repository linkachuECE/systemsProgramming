#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <sys/resource.h>
#include "queue.h"


int fg_pid = 0;
int fg_suspended = 0;
int run = 1;
struct queue pid_list;

enum sp{FCFS, RoundRobin, MFQ};
enum sp scheduling_policy = FCFS;

void help() {
	printf("This is manual page\n");
	printf("This shell supports the following commands:\n");
	printf("\tver\n\texec\n\tps\n\tkill\n\tset_scheduling\n\thelp\n\texit\n");
	printf("For more details please type 'help command'\n");
}

void helpcmd(char *cmd) {
	printf("This is manual page\n\n");
	if (strcmp(cmd,"ver")==0)
	{
		printf("\nver:\tShows details about the shell version\n");
	}
	else if (strcmp(cmd,"exec")==0)
	{
		printf("\nexec p1(n1,qt1) p2(n2,qt2) ...:\nExecutes the programs p1, p2 ...\nEach program types a message for n times and it is given a time quantum of qt msec.\n");
		printf("If parameter (&) is given the program will be executed in the background\n");
	}
	else if (strcmp(cmd,"ps")==0)
	{
		printf("\nps:\tShows the living process with the given pid\n");
	}
	else if (strcmp(cmd,"kill")==0)
	{
		printf("\nkill pid:\tEnds the process with the given pid\n");
	}
	else if (strcmp(cmd,"set_scheduling")==0)
	{
		printf("\nset_scheduling scheduling_policy:\tSets the policy to schedule tasks by. Takes one of three arguments:\n- \"FCFS\"\n- \"Round Robin\"\n- \"MFQ\"\n");
	}
	else if (strcmp(cmd,"help")==0)
	{
		printf("\nhelp:\tYou should know this command by now\n");
	}
	else if (strcmp(cmd,"exit")==0)
	{
		printf("\nexit:\tEnds the experience of working in the new shell\n");
	}
	else 
	{
		printf("\nNo Such command. Type help to see a list of commands\n");
	}
}

void ver() {
	printf("\nNew Shell. Works properly!!\n");
}

void ps() {
	struct node *p;
	printf("\nNEW SHELL presents the following living processes\n");
	printf("\tPID\tNAME\n");
	for (p=pid_list.head; p!=NULL; p=p->next)
	{
		printf("\t%d\t%s\n",p->pid,p->name);
	}
}

void mykill(int pid) {
	kill(pid,SIGTERM);
	printf("You have just killed process %d\n",pid );
}

void myexit() {
	char yesno;
	if (pid_list.head==pid_list.tail)
	{
		run=0;
	}
	else {
		printf("There are still living processes Do you want to kill them? (y/n): ");
		yesno=getchar();
		if (yesno == 'y')
		{
			while(pid_list.head!=pid_list.tail)
			{
				mykill(pid_list.tail->pid);
				usleep(10000);
			}
			run=0;
		}
	}
}

void childdead(int signum) {
	int dead_pid, status;
	
	dead_pid=wait(&status);
	printf("The child %d is dead\n",dead_pid );

	delete(&pid_list,dead_pid);
	printf("\n");
	if (dead_pid==fg_pid)
	{
		fg_pid=0;
	}
}

void susp (int signum) {
	fg_suspended=1;
	printf("All processes supspended\n");
}

void cont (int signum) {
	fg_suspended=0;
	printf("Waking all processes...\n");
	while (fg_pid!=0 && fg_suspended!=1)
		pause();
}

void exec_FCFS(const char input[15][30], int argnum) {
	for (int i=1; i<=argnum; i++){
			
		int j,t,status;
		char *args[10];
		char *temp;
		struct node *p;

		for (j = 0; j < 10; j++)
		{
			args[j]=(char *)malloc(10*sizeof(char));
		}

		strcpy(args[0],strtok(input[i],"(,"));
		for (j=1; (temp=strtok(NULL,",)"))!=NULL; j++) 
			strcpy(args[j],temp);
		printf("\n");
		if (strcmp(args[j-1],"&")==0)
		{
			args[j-1]=NULL;
		}
		else
			args[j]=NULL;
		if ((t=fork())==0)
		{
			execv(args[0],args);
		}
		enqueue(t,args[0],&pid_list);
		if (args[j-1]!=NULL)
		{
			fg_pid=t;
			while(fg_pid!=0 && fg_suspended!=1)
				pause();
		}
	}
}

void exec_RoundRobin(const char input[15][30], int argnum){

}

void exec_MFQ(const char input[15][30], int argnum){

}

void exec(const char input[15][30], int argnum) {
	if(scheduling_policy == FCFS)
		exec_FCFS(input, argnum);
	else if(scheduling_policy == MFQ)
		exec_MFQ(input, argnum);
	else if(scheduling_policy == RoundRobin)
		exec_RoundRobin(input, argnum);
/*
	int i,t,status;
	char *args[10];
	char *temp;
	struct node *p;

	for (i = 0; i < 10; i++)
	{
		args[i]=(char *)malloc(10*sizeof(char));
	}

	strcpy(args[0],strtok(input,"(,"));
	for (i=1; (temp=strtok(NULL,",)"))!=NULL; i++) 
		strcpy(args[i],temp);
	printf("\n");
	if (strcmp(args[i-1],"&")==0)
	{
		args[i-1]=NULL;
	}
	else
		args[i]=NULL;
	if ((t=fork())==0)
	{
		execv(args[0],args);
	}
	enqueue(t,args[0],&pid_list);
	if (args[i-1]!=NULL)
	{
		fg_pid=t;
		while(fg_pid!=0 && fg_suspended!=1)
			pause();
	}
*/
}

void set_scheduling(char *input){
	if(strcmp(input, "FCFS") == 0)
		scheduling_policy = FCFS;
	else if (strcmp(input, "Round") == 0)
		scheduling_policy = RoundRobin;
	else if (strcmp(input, "MFQ") == 0)
		scheduling_policy = MFQ;
	else
		printf("Invalid scheduling policy\n");
}

int main(int argc, char const *argv[]) {
	char input[15][30];
	int argnum, i;

	pid_list.head=NULL;
	pid_list.tail=NULL;
	enqueue(getppid(),"NEW SHELL",&pid_list);

	signal(SIGCHLD,childdead);
	signal(SIGTSTP,susp);
	signal(SIGQUIT,cont);
	setpriority(PRIO_PROCESS,0,-20);

	ver();

	while (run){
		printf("=>");
		for (argnum=0; (scanf("%s",&input[argnum]))==1;argnum++)
			if (getchar()=='\n') break;
		if (strcmp(input[0],"ver")==0 && argnum==0) ver();
		else if (strcmp(input[0],"help")==0 && argnum==0) help();
		else if (strcmp(input[0],"help")==0 && argnum==1) helpcmd(input[argnum]);
		else if (strcmp(input[0],"ps")==0 && argnum==0) ps();
		else if (strcmp(input[0],"kill")==0 && argnum==1) mykill(atoi(input[1]));
		else if (strcmp(input[0],"exec")==0 && argnum!=0) exec(input, argnum);
		else if (strcmp(input[0], "set_scheduling") == 0 && argnum == 1) set_scheduling(input[argnum]);
		else if (strcmp(input[0],"exit")==0 && argnum==0) myexit();
	    else printf("No such command. Check help for help.\n");
	}

}
