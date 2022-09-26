#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define NUM_TRIALS 100000

int nhit;
void* WorkerThread(void* pvar);

int main(int argc, char *argv[]) 
{
   nhit = 0;
   int n = atoi(argv[1]); //input the number of students in class = n
   pthread_t threads[4];

   for(int i = 0; i < 4; i++)
   {
      pthread_create(&threads[i], NULL, WorkerThread, (void *)&n);
      pthread_join(threads[i], NULL);       
   }

   double rate = (double)nhit / (4*NUM_TRIALS);
   double p = rate * 100; //print as a probability

   printf("For a class of %d students, the probability of two students who have the same birthday is about %.2f%%\n", n, p);
   return 0;
}

void *WorkerThread(void *n)
{
   int num = *((int*) n);
   for(int i = 0; i < NUM_TRIALS; i++) 
   {
      int array[num];
      for(int j = 0; j < num; j++) 
      {
         array[j] = rand() % 365;
      }
      int coincide = 0;
      for(int j = 0; j < num; j++) 
      {
         for(int k = j+1; k < num; k++)
         {
            if(array[j]==array[k]) 
               coincide = 1;
         }
      }
      if(coincide == 1) 
         nhit++;
   }

 
   pthread_exit(NULL);
}
