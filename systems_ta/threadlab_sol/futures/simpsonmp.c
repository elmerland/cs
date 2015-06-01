/**
 * Compute Pi (unit circle area) by integrating sqrt(1-x^2) from -1 to 1.
 * Uses adaptive quadrature via Simpson's method.
 *
 * GNU Multi-Precision (mp) version.
 *
 * Remember: 3/14 is Pi day, and this year is 3/14/15.
 *
 * Written for CS 3214 Spring 2015.
 *
 * @author Godmar Back <godmar@gmail.com>
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <gmp.h>
#include "threadpool.h"
#include "threadpool_lib.h"

#define BITS_PER_DIGIT   3.32192809488736234787
#define PI_100 "3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798"

typedef void (*MPFunc)(mpf_t y, mpf_t x);
struct simpsonState {
    MPFunc f;
    mpf_t a, b, epsilon;
    int level_max;
};

/* 
 * @param epsilon - desired error for interval
 * @param level_max - maximum recursion depth
 */
static void *
simpson(struct thread_pool *pool, void *_state) {
    struct simpsonState * state = _state;
    // simpson(DoubleFunc f, double a, double b, double epsilon, int level_max) 
    MPFunc f = state->f;
    mpf_t a, b;
    mpf_init_set(a, state->a);
    mpf_init_set(b, state->b);
    int level_max = state->level_max;

    // double m = (a + b)/2;
    mpf_t m;
    mpf_init_set(m, a);
    mpf_add(m, m, b);
    mpf_div_ui(m, m, 2);

    mpf_t fa, fb, fm;
    // double fa = f(a);   // make sure these are computed only once
    mpf_init(fa);
    f(fa, a);

    // double fb = f(b);
    mpf_init(fb);
    f(fb, b);

    // double fm = f(m);
    mpf_init(fm);
    f(fm, m);

    /*
    double one_simpson = (b - a) * (fa + 4 * fm + fb)/6;
    */
    mpf_t one_simpson;
    mpf_t ba;
    mpf_init_set(ba, b);
    mpf_sub(ba, ba, a);     // b-a

    mpf_init_set(one_simpson, fm);
    mpf_mul_ui(one_simpson, one_simpson, 4);
    mpf_add(one_simpson, one_simpson, fa);
    mpf_add(one_simpson, one_simpson, fb);
    mpf_mul(one_simpson, ba, one_simpson);
    mpf_div_ui(one_simpson, one_simpson, 6);

    /*
    double two_simpson = (b - a) * (fa + 4 * f((a + m)/2) + 2 * fm + 4 * f((m + b)/2) + fb)/12;

    TBD: implement smarter approach that avoids redundant function computation.
    */
    mpf_t two_simpson;
    mpf_init_set(two_simpson, fm);
    mpf_mul_ui(two_simpson, two_simpson, 2);
    mpf_add(two_simpson, two_simpson, fa);
    mpf_add(two_simpson, two_simpson, fb);

    mpf_t fam2, am2;
    mpf_init(fam2);
    mpf_init(am2);
    mpf_add(am2, a, m);
    mpf_div_ui(am2, am2, 2);
    f(fam2, am2);
    mpf_mul_ui(fam2, fam2, 4);
    mpf_add(two_simpson, two_simpson, fam2);

    mpf_t fbm2, bm2;
    mpf_init(fbm2);
    mpf_init(bm2);
    mpf_add(bm2, m, b);
    mpf_div_ui(bm2, bm2, 2);
    f(fbm2, bm2);
    mpf_mul_ui(fbm2, fbm2, 4);
    mpf_add(two_simpson, two_simpson, fbm2);

    mpf_mul(two_simpson, ba, two_simpson);
    mpf_div_ui(two_simpson, two_simpson, 12);

    mpf_clear(fbm2);
    mpf_clear(bm2);
    mpf_clear(fam2);
    mpf_clear(am2);
    mpf_clear(ba);

    mpf_t * result = malloc(sizeof *result);

    if (level_max == 0) { 
        mpf_init_set(*result, two_simpson); // too deep
    } else {
        mpf_t d, e15;
        mpf_init(d);
        mpf_init(e15);
        mpf_sub(d, two_simpson, one_simpson);
        mpf_abs(d, d);
        mpf_mul_ui(e15, state->epsilon, 15);
        int cmprc = mpf_cmp(d, e15);
        mpf_clear(e15);
        mpf_clear(d);
        // if (fabs(two_simpson - one_simpson) < 15*epsilon) {
        if (cmprc < 0) {
            // result = 16 * two_simpson / 15 - one_simpson / 15;
            mpf_mul_ui(two_simpson, two_simpson, 16);
            mpf_div_ui(two_simpson, two_simpson, 15);
            mpf_div_ui(one_simpson, one_simpson, 15);
            mpf_init(*result);
            mpf_sub(*result, two_simpson, one_simpson);
        } else {
            struct simpsonState leftTask = {
                .f = f,
                // .a = a,
                // .b = m,
                // .epsilon = epsilon/2,
                .level_max = level_max - 1
            };
            mpf_init_set(leftTask.a, a);
            mpf_init_set(leftTask.b, m);
            mpf_init(leftTask.epsilon);
            mpf_div_ui(leftTask.epsilon, state->epsilon, 2);

            struct future *leftFuture = thread_pool_submit(pool, simpson, &leftTask);
            struct simpsonState rightTask = {
                .f = f,
                // .a = m,
                // .b = b,
                // .epsilon = epsilon/2,
                .level_max = level_max - 1
            };
            mpf_init(rightTask.epsilon);
            mpf_init_set(rightTask.a, m);
            mpf_init_set(rightTask.b, b);
            mpf_div_ui(rightTask.epsilon, state->epsilon, 2);

            mpf_t * right = simpson(pool, &rightTask);
            mpf_clear(rightTask.a);
            mpf_clear(rightTask.b);
            mpf_clear(rightTask.epsilon);

            mpf_t * left = future_get(leftFuture);
            mpf_clear(leftTask.a);
            mpf_clear(leftTask.b);
            mpf_clear(leftTask.epsilon);
            future_free(leftFuture);

            mpf_init(*result);
            mpf_add(*result, *left, *right);

            mpf_clear(*left);
            free(left);

            mpf_clear(*right);
            free(right);
        }
    }
    mpf_clear(a);
    mpf_clear(b);
    mpf_clear(m);
    mpf_clear(fa);
    mpf_clear(fb);
    mpf_clear(fm);
    mpf_clear(one_simpson);
    mpf_clear(two_simpson);
    return result;
}

// return sqrt(1 - x * x);
static void
sqrt1x2(mpf_t y, mpf_t x)
{
    mpf_t tmp;
    mpf_init_set(tmp, x);       // tmp = x
    mpf_mul(tmp, tmp, x);       // tmp = tmp * x
    mpf_ui_sub(tmp, 1, tmp);    // tmp = 1 - tmp
    mpf_sqrt(y, tmp);           // y = sqrt(tmp)
    mpf_clear(tmp);
}

static void benchmark(int ndigits, int level_max, int threads)
{
    long int prec = ndigits * BITS_PER_DIGIT + 16;
    mpf_set_default_prec(prec);
    fprintf(stderr, "Attempting %d digits, setting mantissa precison to %ld bits\n", ndigits, prec);

    mpf_t eps;  // 10^{-ndigts}
    mpf_init_set_si(eps, 10);           // eps = 10
    mpf_pow_ui(eps, eps, ndigits);      // eps = 10^ndigits
    mpf_ui_div(eps, 1, eps);            // eps = 1/10^ndigits

    struct thread_pool* pool = thread_pool_new(threads);

    struct benchmark_data* bdata = start_benchmark();
    
    struct simpsonState rootTask = {
        .f = sqrt1x2,
        .level_max = level_max
    };
    mpf_init_set_si(rootTask.a, -1);
    mpf_init_set_ui(rootTask.b, 1);
    mpf_init_set(rootTask.epsilon, eps);

#if 0
    printf("eps=");
    mpf_out_str(stdout, 10, ndigits, rootTask.epsilon);
    printf("\n");
#endif

    struct future* fut = thread_pool_submit(pool, simpson, &rootTask);
    mpf_t * answer = (mpf_t *) future_get(fut);

    mpf_clear(eps);
    mpf_clear(rootTask.a);
    mpf_clear(rootTask.b);

    stop_benchmark(bdata);
    free(bdata);

    future_free(fut);
    thread_pool_shutdown_and_destroy(pool);

    mpf_t pi, error;
    mpf_init_set_str(pi, PI_100, 10);
    mpf_mul_ui(*answer, *answer, 2);
    mpf_init(error);
    mpf_sub(error, *answer, pi);

    printf("Solution: ");
    mpf_out_str(stdout, 10, 100, *answer);
    printf("\n           "PI_100"\n");
    printf("Error:");
    mpf_out_str(stdout, 10, 5, error);
    printf("\n");

    mpf_clear(*answer);
    free(answer);

    mpf_abs(error, error);
    if (mpf_cmp(error, rootTask.epsilon) <= 0) {
        printf("Solution ok.\n");
        report_benchmark_results(bdata);
    }
    else { 
        fprintf(stderr, "Solution bad.\n");
        abort();
    }

    mpf_clear(rootTask.epsilon);
    mpf_clear(pi);
    mpf_clear(error);
}

static void usage(char *av0, int depth, int nthreads, int ndigits) {
    fprintf(stderr, "Usage: %s [-d <n>] [-n <n>] [-l <d>]\n"
                    " -d        maximum recursion depth, default %d\n"
                    " -n        number of threads in pool, default %d\n"
                    " -l <d>    number of digits of pi we wish to compute, default %d\n"
                    , av0, depth, nthreads, ndigits);
    abort();
}

int main(int ac, char** av) {

    if (sizeof(void *) != sizeof(double)) {
        fprintf(stderr, "This is not portable code, please compile under 64-bit pointer model\n");
        abort();
    }
    install_stack_overflow_handler();

    int threads = 4;
    int ndigits = 100;
    int max_parallel_depth = 30;
    int c;
    while ((c = getopt(ac, av, "l:d:n:he:")) != EOF) {
        switch (c) {
        case 'd':
            max_parallel_depth = atoi(optarg);
            break;
        case 'l':
            ndigits = atoi(optarg);
            break;
        case 'n':
            threads = atoi(optarg);
            break;
        case 'h':
            usage(av[0], max_parallel_depth, threads, ndigits);
        }
    }
/*
    if (optind == ac)
        usage(av[0], max_parallel_depth, threads, ndigits);
*/

    benchmark(ndigits, max_parallel_depth, threads);
    return 0;
    
}
