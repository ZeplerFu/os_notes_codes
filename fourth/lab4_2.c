#include<stdio.h>
#include<unistd.h>
int main()
{
	pid_t pid;
	pid = fork();	//fork a process
	if(pid == 0) 
	{               
		printf("this is child process, pid is %d\n",getpid());
		
	} 	
	else if(pid > 0) 
	{
		printf("this is father process, pid is %d\n",getpid());
		
	}      
	else 
	{
		fprintf(stderr,"ERROR:fork() failed!\n");
	}
	sleep(10);//pause here for 10 seconds, let me have time to use "ps aux" command to see the process list
	return 0;
}

