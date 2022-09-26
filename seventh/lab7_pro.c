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
#include <errno.h>

#define BUFFER_LEN 10

int n = 1;

int main(int argc, char *argv[]) {

    n = atoi(argv[1]);
    //printf("n in producer = %d\n", n);
    printf("address of n in producer: %p\n", &n);

      /* the size of shared memory*/
	int LEN = n * sizeof(float) + 2 * sizeof(int);

	/* name of the shared memory*/
	const char* name = "lab7_shm";

	int shm_fd;   // file descriptor
	void* ptr;    //pointer to shared memory
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) 
	{
        printf("shared memory of producer failed: %s\n", strerror(errno));
        exit(1);
        }
	ftruncate(shm_fd, LEN);
	ptr = mmap(0, LEN, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        printf("start address of the shared buffer from producer = %p \n",ptr);
     

      int *in = (int*) ptr;
      *in = 0;
      int *out = in+1;
      *out = 0;
      float (*buffer);

      buffer = (float*)(out+1);

      for(int y=0; y<n; y++) 
        {

            float z = 0.25 * y * y;
            while((*in+1) % BUFFER_LEN == *out);
            buffer[*in] = z;
            *in = (*in+1) % BUFFER_LEN;
        }
    

  return 0;
}
