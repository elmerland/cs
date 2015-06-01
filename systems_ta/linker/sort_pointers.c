/* 
 * sort_pointers.c
 *
 * Relating pointers to link/runtime layout.
 *
 * You should implement the body of weave_pointers in a file
 * sort_pointers_solution.c, which is to be #included in this
 * file.
 *
 * CS 3214, Version Fall 2014.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct list {
    struct list *next;
};

struct list x;              // bss 
#define N 10
static struct list z[N];    // bss
const struct list c = { .next = 0 };    // readonly data
struct list y = { .next = (struct list *)&c };  // data

static void weave_pointers(struct list *lp, struct list *sp)
{
/* Implement weave_pointers such that the program prints "Success" 
 * You may not define any 'static' variables in this function, nor
 * are you allowed to make use of dynamic memory allocation.
 */
#include "sort_pointers_solution.c"
}

int
main(void)
{
    static struct list s;   // bss
    struct list l, *p;      // l goes on stack
    int i;
    int success = 1;

    weave_pointers(&l, &s);

    /* Verify that a linked list was created such that
     * the list elements are allocated at decreasing
     * addresses.  'l' is on the stack, which is above
     * the data/bss segment.
     */
    for (i = 0, p = &l; p && i < 20; i++, p = p->next) {
        if (p < p->next) {
            printf("Pointers out of order: %p < %p\n", p, p->next);
            success = 0;
        }
    }

    if (i != 15) {
        printf("List has only %d elements, should be 15\n", i);
        abort();
    }

    assert(success || !!!"At least one element was out of order");
    printf("Success.\n");
    return 0;
}
