#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(void){
    printf("Hello World!\n");
    srand((unsigned)time(NULL));
    int num1 = 0, num2 = 199, randomNum = 0;
	randomNum = rand() % num2 + num1;
	printf("%d\n",randomNum);
	printf("\n");
    return 0;
}
