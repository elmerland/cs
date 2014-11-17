#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "prime.h"
#include "rngs.h"

/* Developed by Dennis Kafura for CS3214 in Spring, 2014 */

#define READ_FROM_PIPE  0
#define WRITE_TO_PIPE   1

int nwrites= 0;    // number of writes to pipe by rand-gen

int in_pipe(fd) {
  int nleft=0;
  int rnum;
  while(read(fd, &rnum, sizeof(int)))
    nleft++;
  return nleft;
}

void* prime_test(void * fdarg) // read from number from pipe and test for prime
{
    int fd = *(int*)fdarg;
    int rnum;
    int tested = 0;
    int primes = 0;
 
    while(primes<1000) {
       read(fd, &rnum, sizeof(int)); 
       tested++;
       if(is_prime(rnum)) primes++;
    }

    int* result = (int*) malloc(sizeof(int));
    *result = tested;
    pthread_exit(result);
}

void* rand_gen(void * fdarg) // write to pipe a sequence of random numbers
{
    int fd = *(int*)fdarg;

    int rnum;

    while(1){
     rnum = Random()*1000000;
     int nw = write(fd, &rnum, sizeof(int));
     if(nw>0) nwrites++;
    }
}

int
main()
{
    InitRandom();  // initialize the random number generator

    int pipefds[2];
    pipe(pipefds);
    
    int* result[3];
    pthread_t t[3];

    pthread_create(&t[0], NULL, rand_gen,   &pipefds[WRITE_TO_PIPE]);
    pthread_create(&t[1], NULL, prime_test, &pipefds[READ_FROM_PIPE]);
    pthread_create(&t[2], NULL, prime_test, &pipefds[READ_FROM_PIPE]);

    pthread_join(t[1], (void*)&result[1]);
    pthread_join(t[2], (void*)&result[2]);
    pthread_cancel(t[0]);
    close(pipefds[WRITE_TO_PIPE]);
    int nleft = in_pipe(pipefds[READ_FROM_PIPE]);

    printf("Numbers written to pipe:   %d \n",    nwrites);
    printf("Numbers read by thread 1:  %d \n", *result[1]);
    printf("Numbers read by thread 2:  %d \n", *result[2]);
    printf("Numbers remaining in pipe: %d \n",      nleft);

    return 0;
}
