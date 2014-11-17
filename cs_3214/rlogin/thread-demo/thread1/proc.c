#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/* Developed by Dennis Kafura in Spring, 2014 for CS3214 */

int count = 0;

int main() {

pid_t pid1, pid2;

  if((pid1=fork()) == 0) {

    printf("Child 1 starting. \n");
    int i;
    for(i=0; i<10000; i++)
      count++;

    printf("Child 1 terminated after incrementing count 10000 times. \n");
    exit(0);
  }

  if((pid2=fork()) == 0) {

    printf("Child 2 starting. \n");
    int i;
    for(i=0; i<10000; i++)
      count++;

    printf("Child 2 terminated after incrementing count 10000 times. \n");
    exit(0);
  }

  wait(pid1);
  wait(pid2);

  printf("Both child processes have terminated.\n");
  printf("Final value of count is %d \n", count);
}
