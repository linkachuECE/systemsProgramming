#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/resource.h>

void cont (int sig_num) {
     
}

void stop (int sig_num) {
     pause();
}


int main (char *argc[], char *argv[]) {


	int i,num,sltime;

	signal(SIGQUIT,cont);
	signal(SIGTSTP,stop);
	
	num =atoi(argv[1]);
	sltime = 1000*atoi(argv[2]);
	
	for (i=1; i<=num; i++){
		printf("This is program %s and it prints for the %d time of %d...\n",argv[0],i,num);
		usleep(sltime);
	}
}
