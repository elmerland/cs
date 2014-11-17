#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "prime.h"
#include "rngs.h"
#include "int-pipe.h"


/* Developed by Dennis Kafura for CS3214 in Spring, 2014 */


int_pipe_t ipipe;

bool done = false;        // tells rand-gen when to stop producing randome numbers


void* prime_test(void *arg)
								 // read from int_pipe and count primes
{
	int_pipe_t *intp = (int_pipe_t*)arg;
	int rnum;
	int tested = 0;
	int primes = 0;

	while(primes<1000) {
		rnum = int_pipe_read(intp);
		tested++;
		if(is_prime(rnum)) primes++;
	}

	int* result = (int*)malloc(sizeof(int));
	*result = tested;
	pthread_exit(result);
}

void* rand_gen(void *arg) // write to int_pipe a sequence of random numbers
{
	int_pipe_t *intp = (int_pipe_t*)arg;

	int nwrites=0;
	int rnum;
	pthread_t id = pthread_self();

	while(!done) {
		// printf("[%d] Start rand\n", id);
		rnum = Random()*1000000;
		int nw = int_pipe_write(intp,rnum);
		if (nw>0) nwrites++;
		// printf("[%d] End rand\n", id);
	}
	// printf("exit rand\n");
	int* result = (int*) malloc(sizeof(int));
	*result=nwrites;
	pthread_exit(result);

}

int main()
{
	InitRandom();  // initialize the random number generator

	int_pipe_init(&ipipe);

	int*  result[5];
	pthread_t  t[5];

	pthread_create(&t[0], NULL, rand_gen,   (void*)&ipipe);
	pthread_create(&t[3], NULL, rand_gen,   (void*)&ipipe);
	pthread_create(&t[4], NULL, rand_gen,   (void*)&ipipe);
	pthread_create(&t[1], NULL, prime_test, (void*)&ipipe);
	pthread_create(&t[2], NULL, prime_test, (void*)&ipipe);

	pthread_join(t[1], (void*)&result[1]);
	pthread_join(t[2], (void*)&result[2]);

	done=true;

	int_pipe_close(&ipipe);

	pthread_join(t[0], (void*)&result[0]);
	pthread_join(t[3], (void*)&result[3]);
	pthread_join(t[4], (void*)&result[4]);

	int nleft = int_pipe_size(&ipipe);

	printf("Numbers written to pipe 1:   %d \n",  *result[0]);
	printf("Numbers written to pipe 2:   %d \n",  *result[3]);
	printf("Numbers written to pipe 3:   %d \n",  *result[4]);
	printf("Numbers read by thread 1:  %d \n",  *result[1]);
	printf("Numbers read by thread 2:  %d \n",  *result[2]);
	printf("Numbers remaining in pipe: %d \n",  nleft);
	int temp = *result[1] + *result[2] + nleft;
	int temp2 = *result[0] + *result[3] + *result[4];
	printf("%s\n", temp == temp2 ? "OK": "ERR");

	return 0;
}
