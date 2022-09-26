#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <time.h>
#include <sched.h>
#include <string.h>

int main(int argc, char *argv[])
{

    int n = atoi(argv[1]);
    int fd[2];
    
    if(pipe(fd) == -1) 
    {
        fprintf(stderr,"ERROR:pipe failed");
        return -1;
    }

    pid_t pid;
    pid = fork();	
    
  if(pid == 0)  //child process
  {      
       close(fd[0]);

        for(int y=0;y<n;y++)
        {
            float z=0.25*y*y;
            char str[10];
            sprintf(str,"%.2f  ",z);
            write(fd[1],str,strlen(str)+1);
            useconds_t t = random() % 5000;
            usleep(t * 1000);
        }
  } 	
  else if(pid > 0)  //parent process
  {
	close(fd[1]);
	
        char msg[10];
        for(int y = 0; y < n; y++) 
        {
            memset(msg, '\0', sizeof(msg));
            read(fd[0], msg, sizeof(msg));
            printf("%s", msg);
            fflush(stdout);
        }
        printf("\n");
       close(fd[0]);	
  }     
  else 
  {
	fprintf(stderr,"ERROR:fork failed!\n");
  }
    return 0;
}


