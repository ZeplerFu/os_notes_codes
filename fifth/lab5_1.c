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

#define BUFFER_LEN 10

int main(int argc, char *argv[]) 
{


    int n = atoi(argv[1]);
    int LEN = n * sizeof(float) + 2 * sizeof(int);

    const char* name = "lab5_1";
    int shm_fd;
    void *ptr;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, LEN);
    ptr = mmap(0, LEN, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    int *in = (int*) ptr;
    *in = 0;
    int *out = in + 1;
    *out = 0;
    float (*buffer);
    buffer = (float*)(out+1);


    pid_t pid;
    pid = fork();

    if(pid == 0)  //child process
    {
        for(int y=0; y<n; y++) 
        {
            float z = 0.25 * y * y;
            while((*in+1) % BUFFER_LEN == *out);
            buffer[*in] = z;
            *in = (*in+1) % BUFFER_LEN;
            useconds_t t = random() % 5000;
            usleep(t * 1000);
        }
    }
    else if (pid > 0)  //parent process
    {
        for(int y=0; y<n; y++) 
        {
            while(*in == *out);
            printf("%.2f  ", buffer[*out]);
            fflush(stdout);
            *out = (*out+1) % BUFFER_LEN;
        }
        printf("\n");
        shm_unlink(name);
    }
    else 
    {
        fprintf(stderr,"ERROR:fork failed");
        return 1;
    } 

    return 0;
 }
