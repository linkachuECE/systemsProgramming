#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 *	AUTHOR: Ethan Braun
 *	DATE CREATED: 02/15/23
 *	DESCRIPTION: The purpose of this program is to demonstrate the fork() function
 *	CONTRIBUTORS: None. The code is identical to what I downloaded from D2L
 */


int main(int argc, char const *argv[])
{
	
	printf("Now I will try to create a child\n");
	if (fork() == 0)
	{
		printf("Hello, I am a child with PID: %d\n", getpid());
		exit(0);
	}
	printf("Father is done, my PID is %d\n",getpid());
	
	return 0;
}

