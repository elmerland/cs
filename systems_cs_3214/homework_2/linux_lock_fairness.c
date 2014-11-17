#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

/*
compile using the following command

gcc -Wall -Werror -std=c99 -lpthread -o linux_fairness_test linux_lock_fairness.c
 */

// Lock for the counters integer array.
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// Lock for the index variable.
pthread_mutex_t index_lock = PTHREAD_MUTEX_INITIALIZER;

// Integer array that holds the counters for each thread.
int * counters;
/**
 * The index value that indicates to each thread what
 * index in the counters array they are going to store
 * their counters are in.
 */
int index = 0;
// Flag that tells the threads to stop.
bool stop = false;

/**
 * Function to be excuted by thread.
 * @param  arg NULL
 * @return     NULL
 */
static void * locking(void * arg) {
	// Initialize thread copy of index.
	int my_index;
	// Get coopy of index and increase index for next thread.
	pthread_mutex_lock(&index_lock);
	my_index = index++;
	pthread_mutex_unlock(&index_lock);

	// Infinite while loop that gets a lock.
	while(!stop) {
		// Get lock and increase counter.
		pthread_mutex_lock(&lock);
		counters[my_index]++;
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

/**
 * Main function that starts N threads and makes the threads fight for a lock.
 * @param  argc Number of arguments.
 * @param  argv Array of strings with arguments.
 * @return      Nothing.
 */
int main(int argc, char ** argv) {
	// Get the number of threads
	int NTHREADS = strtol(argv[1], NULL, 10);
	printf("Threads: %d\n", NTHREADS);
	// Allocate memory for the integer array.
	counters = malloc(sizeof(int) * NTHREADS);
	// Initialize all the threads.
	pthread_t threads[NTHREADS];
	for (int i = 0; i < NTHREADS; i++) {
		pthread_create(&threads[i], NULL, locking, NULL);
	}
	// Wait for all threads to get started
	sleep(1);
	// Reset the counters
	pthread_mutex_lock(&lock);
	for (int i = 0; i < NTHREADS; i++) {
		counters[i] = 0;
	}
	pthread_mutex_unlock(&lock);
	// Wait three seconds for all threads to run for a while.
	sleep(3);
	// Get the counters for every thread.
	pthread_mutex_lock(&lock);
	double max = 0;
	double min = counters[0]/3;
	int total = 0;
	for (int i = 0; i < NTHREADS; i++) {
		total += counters[i];
		double lock_p_sec = (double)counters[i]/3;
		if (min > lock_p_sec) {
			min = lock_p_sec;
		}
		if (max < lock_p_sec) {
			max = lock_p_sec;
		}
		printf("[%2d] %'12.2f\n", i, lock_p_sec);
	}
	printf("Total locks: %'12d\n", total);
	printf("Per second:  %'12d\n", total/3);
	printf("Perc. diff.:\t  %'6.2f%%\n", ((max - min)/((max + min)/2)) * 100);
	// Set flag to stop threads from running.
	stop = true;
	pthread_mutex_unlock(&lock);
	// Reap all threads
	for (int i = 0; i < NTHREADS; i++) {
 		pthread_join(threads[i], NULL);
	}
	// Free the integer array.
	free(counters);
	return 0;
}
