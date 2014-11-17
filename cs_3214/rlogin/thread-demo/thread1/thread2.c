#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* Originally written by Gomar Back for CS3214.
 * Modified by Dennis Kafura in Spring, 2014.
 * */

void print_msg(char*  msg) {
  write(1, msg, strlen(msg));
}

int count=0;

void *
thread1(void * none)
{
    print_msg("Thread 1 starting.\n");
    int i;
    for(i=0; i<10000; i++)
       count=count+1;

    print_msg("Thread 1 terminated after incrementing count by 10000.\n");
    return NULL;
}

void *
thread2(void * none)
{
    print_msg("Thread 2 starting.\n");
    int i;
    for(i=0; i<10000; i++)
       count++;
    
    print_msg("Thread 2 terminated after incrementing count by 10000.\n");
    return NULL;
}

int
main()
{
    int i;
    pthread_t t[2];
    

    pthread_create(&t[0], NULL, thread1, NULL);
    pthread_create(&t[1], NULL, thread2, NULL);


    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);

    printf("Both threads have terminated. \n");
    printf("Final value of count is %d\n", count);
    return 0;
}
