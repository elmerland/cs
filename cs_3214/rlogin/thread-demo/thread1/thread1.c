#include <pthread.h>
#include <stdio.h>
#include <string.h>


/* Developed by Dennis Kafura for CS3214 in Spring, 2014 */

void print_msg(char* msg){
   write(1, msg, strlen(msg));
}

int count;

static void *
thread1(void * _tn)
{
    print_msg("Thread 1 starting.\n");
    int i;
    for(i=0; i<10000; i++)
       count++;
    print_msg("Thread 1 terminated after incrementing count 10000 times. \n");
    return NULL;
}

static void *
thread2(void * _tn)
{
    print_msg("Thread 2 starting.\n");
    int i;
    for(i=0; i<10000; i++)
       count++;
    print_msg("Thread 2 terminated after incrementing count 10000 times. \n");
    return NULL;
}

int
main()
{
    int i, N = 2;
    pthread_t t[N];
    pthread_create(&t[0], NULL, thread1, NULL);
    pthread_join(t[0], NULL);

    pthread_create(&t[1], NULL, thread2, NULL);
    pthread_join(t[1], NULL);

    printf("Both threads have terminated. \n");
    printf("Final value of count is %d\n", count);
    return 0;
}
