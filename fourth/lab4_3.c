#include <sys/mman.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include<sys/types.h> 
#include<fcntl.h> /* For O_* constants */
#include<sys/stat.h> /* For mode constants */
#include <string.h>

#define BUFFER_SIZE 256
    typedef struct
    {
    float data[BUFFER_SIZE];
    int in;
    int out;
    } shared;
    
    shared buffer[BUFFER_SIZE];
    int in=0;
    int out=0;
    

int main(int argc, char *argv[])
{

    int n = atoi(argv[1]);
    
    const char *name = "lab4_3";
    int shm_fd;
    void *ptr;
    shm_fd = shm_open(name, O_CREAT|O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shared));
    shared* q;
    q = (shared*) ptr;
    ptr = mmap(0, sizeof(shared),PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pid_t pid;
    pid = fork();	
  if(pid == 0)  //child process
  {      
		for(int y=0;y<n;y++)
    {
      while(((q->in + 1)%(BUFFER_SIZE)) == q->out);
      float z = 0.25 * y * y;
      q->data[q->in] = z;
      q->in = ((q->in +1)%(BUFFER_SIZE));
      printf("%.2f ", z);
      fflush(stdout);
      useconds_t t = random() % 5000;
      usleep(t * 1000);    
    }	
  } 	
  else if(pid > 0)  //parent process
  {
		 for (;;)
       {
             while(q->in == q->out);
            float z = q->data[q->out];
	     q->out = ((q->out +1)%(BUFFER_SIZE));
	     if (z == -1) break;
             printf("%.2f ", z);
	}		
  }     
  else 
  {
	fprintf(stderr,"ERROR:fork() failed!\n");
  }
    return 0;
}


