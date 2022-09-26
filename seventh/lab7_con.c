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
    //printf("n in consumer = %d\n", n);
    printf("address of n in consumer: %p\n", &n);

    int shm_fd;   // file descriptor
    void *ptr;    //pointer to shared memory

    int LEN = n * sizeof(float) + 2 * sizeof(int);
    
    const char* name = "lab7_shm";
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) 
    {
        printf("shared memory of consumer failed: %s\n", strerror(errno));
        exit(1);
    }
    ftruncate(shm_fd, LEN);
    ptr = mmap(0, LEN, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    printf("start address of the shared buffer from consumer = %p \n",ptr);
    

    int *in = (int*) ptr;
    int *out = in + 1;
    float (*buffer);
    buffer = (float*)(out+1);

    for(int y=0; y<n; y++) 
        {
            while(*in == *out);
            printf("%.2f  ", buffer[*out]);
            fflush(stdout);
            usleep(rand()%5000000);
            *out = (*out+1) % BUFFER_LEN;
        }
        printf("\n");
        shm_unlink(name);

    
  return 0;
}
