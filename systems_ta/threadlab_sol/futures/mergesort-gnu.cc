/*
 * Parallel Mergesort.
 *
 * Demo application that shows how one might use threadpools/futures
 * in an application.
 *
 * Requires threadpool.c/threadpool.h
 *
 * Written by Godmar Back gback@cs.vt.edu for CS3214 Fall 2014.
 */

#define _GLIBCXX_PARALLEL
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <assert.h>
#include <getopt.h>
#include <vector>
#include <algorithm>
#include <iostream>

#define INSERTION_SORT_THRESHOLD    16
static int insertion_sort_threshold = INSERTION_SORT_THRESHOLD;

extern "C" {
#include "threadpool.h"
}
static struct thread_pool * threadpool;

typedef void (*sort_func)(int *, int);
typedef void (*array_sort_func)(std::vector<int> &);

/* Return true if array 'a' is sorted. */
static bool
check_sorted(int a[], int n) 
{
    int i;
    for (i = 0; i < n-1; i++)
        if (a[i] > a[i+1])
            return false;
    return true;
}

/* Return true if std::vector 'a' is sorted. */
static bool
check_sorted(std::vector<int> a)
{
    for (std::vector<int>::size_type i = 0; i < a.size() - 1; i++)
        if (a[i] > a[i+1])
            return false;
    return true;
}

/* ------------------------------------------------------------- 
 * Built-in qsort.
 */
static int cmp_int(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

static void builtin_qsort(int *a, int N)
{
    qsort(a, N, sizeof(int), cmp_int);
}

/* ------------------------------------------------------------- 
 * GNU parallel sort
 */
static void gnuparallel_sort(std::vector<int> &v)
{
/*
    omp_set_num_threads(16);
    std::cout << " Number of processors available:" << omp_get_num_procs() << " MAX Number of threads " << omp_get_max_threads() << std::endl;
*/
    __gnu_parallel::sort(v.begin(), v.end());
}

/* ------------------------------------------------------------- 
 * Utilities: insertion sort.
 */
static void insertionsort(int *a, int lo, int hi) 
{
    int i;
    for (i = lo+1; i <= hi; i++) {
        int j = i;
        int t = a[j];
        while (j > lo && t < a[j - 1]) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = t;
    }
}

static void
merge(int * a, int * b, int bstart, int left, int m, int right)
{
    if (a[m] <= a[m+1])
        return;

    memcpy(b + bstart, a + left, (m - left + 1) * sizeof (a[0]));
    int i = bstart;
    int j = m + 1;
    int k = left;

    while (k < j && j <= right) {
        if (b[i] < a[j])
            a[k++] = b[i++];
        else
            a[k++] = a[j++];
    }
    memcpy(a + k, b + i, (j - k) * sizeof (a[0]));
}

/* ------------------------------------------------------------- 
 * Serial implementation.
 */
static void
mergesort_internal(int * array, int * tmp, int left, int right)
{
    if (right - left < insertion_sort_threshold) {
        insertionsort(array, left, right);
    } else {
        int m = (left + right) / 2;
        mergesort_internal(array, tmp, left, m);
        mergesort_internal(array, tmp, m + 1, right);
        merge(array, tmp, 0, left, m, right);
    }
}

static void
mergesort_serial(int * array, int n)
{
    if (n < insertion_sort_threshold) {
        insertionsort(array, 0, n);
    } else {
        int * tmp = (int *) malloc(sizeof(int) * (n / 2 + 1));
        mergesort_internal(array, tmp, 0, n);
        free (tmp);
    }
}

/* ------------------------------------------------------------- 
 * Parallel implementation.
 */

/* msort_task describes a unit of parallel work */
struct msort_task {
    int *array;
    int *tmp;
    int left, right;
}; 

/* Parallel mergesort */
static void  
mergesort_internal_parallel(struct thread_pool * threadpool, struct msort_task * s)
{
    int * array = s->array;
    int * tmp = s->tmp;
    int left = s->left;
    int right = s->right;

    if (right - left < insertion_sort_threshold) {
        insertionsort(array, left, right);
    } else {
        int m = (left + right) / 2;

        struct msort_task mleft = {
            array, tmp, left, m
        };
        struct future * lhalf = thread_pool_submit(threadpool, 
                                   (fork_join_task_t) mergesort_internal_parallel,  
                                   &mleft);

        struct msort_task mright = {
            array, tmp, m+1, right
        };
        mergesort_internal_parallel(threadpool, &mright);
        future_get(lhalf);
        future_free(lhalf);
        merge(array, tmp, left, left, m, right);
    }
}

static void 
mergesort_parallel(int *array, int N) 
{
    int * tmp = (int *) malloc(sizeof(int) * (N));
    struct msort_task root = {
        array, tmp, 0, N
    };

    mergesort_internal_parallel(threadpool, &root);
    free (tmp);
}

/*
 * Benchmark one run of sort_func sorter
 */
static void 
benchmark2(const char *benchmark_name, array_sort_func sorter, int *a0, int N)
{
    struct timeval start, end, diff;

    std::vector<int> a(a0, a0 + N);

    gettimeofday(&start, NULL);
    sorter(a);
    gettimeofday(&end, NULL);

    if (!check_sorted(a)) {
        fprintf(stderr, "Sort failed\n");
        abort();
    }
    timersub(&end, &start, &diff);
    printf("%-20s took %.3f sec.\n", benchmark_name, diff.tv_sec + diff.tv_usec / 1.0e6);
}
/*
 * Benchmark one run of sort_func sorter
 */
static void 
benchmark(const char *benchmark_name, sort_func sorter, int *a0, int N)
{
    struct timeval start, end, diff;

    int *a = (int *) malloc(N * sizeof(int));
    memcpy(a, a0, N * sizeof(int));

    gettimeofday(&start, NULL);
    sorter(a, N);
    gettimeofday(&end, NULL);

    if (!check_sorted(a, N)) {
        fprintf(stderr, "Sort failed\n");
        abort();
    }
    timersub(&end, &start, &diff);
    printf("%-20s took %.3f sec.\n", benchmark_name, diff.tv_sec + diff.tv_usec / 1.0e6);
    free(a);
}

static void
usage(char *av0, int thold, int nthreads)
{
    fprintf(stderr, "Usage: %s [-i <n>] [-n <n>] [-b] [-q] [-s <n>] <N>\n"
                    " -i        insertion sort threshold, default %d\n"
                    " -n        number of threads in pool, default %d\n"
                    " -b        run built-in qsort\n"
                    " -s        specify srand() seed\n"
                    " -q        don't run serial qsort\n"
                    , av0, thold, nthreads);
    exit(0);
}

int 
main(int ac, char *av[]) 
{
    int nthreads = 4;
    int c;
    bool run_builtin_qsort = false;
    bool run_serial_msort = true;

    while ((c = getopt(ac, av, "i:n:bhs:q")) != EOF) {
        switch (c) {
        case 'i':
            insertion_sort_threshold = atoi(optarg);
            break;
        case 'n':
            nthreads = atoi(optarg);
            break;
        case 's':
            srand(atoi(optarg));
            break;
        case 'b':
            run_builtin_qsort = true;
            break;
        case 'q':
            run_serial_msort = false;
            break;
        case 'h':
            usage(av[0], insertion_sort_threshold, nthreads);
        }
    }
    if (optind == ac)
        usage(av[0], insertion_sort_threshold, nthreads);

    int N = atoi(av[optind]);

    int i, * a0 = (int *) malloc(N * sizeof(int));
    for (i = 0; i < N; i++)
        a0[i] = random();

    if (run_builtin_qsort)
        benchmark("Built-in qsort", builtin_qsort, a0, N);

    if (run_serial_msort)
        benchmark("mergesort serial", mergesort_serial, a0, N);

    benchmark2("gnu parallel sort", gnuparallel_sort, a0, N);

    threadpool = thread_pool_new(nthreads);
    printf("Using %d threads, insertion sort threshold =%d\n", nthreads, insertion_sort_threshold);
    sleep(1);

    benchmark("mergesort parallel", mergesort_parallel, a0, N);

    thread_pool_shutdown_and_destroy(threadpool);
    return 0;
}

