#include <stdio.h>

struct list_elem
{
	struct list_elem *prev;     /* Previous list element. */
	struct list_elem *next;     /* Next list element. */
};

struct boundary_tag {
    int inuse:1;        // inuse bit
    int size:31;        // size of block, in words
};

struct block {
    struct boundary_tag header; /* offset 0, at address 4 mod 8 */
    char payload[0];            /* offset 4, at address 0 mod 8 */
};

/* Given a block, obtain its footer boundary tag */
static struct boundary_tag * get_footer(struct block *blk) {
    return (void *)((size_t *)blk + blk->header.size)
                   - sizeof(struct boundary_tag);
}

/* Given a block, obtain its footer boundary tag */
static struct boundary_tag * get_footer(struct block *blk) {
    return (void *)((size_t *)blk + blk->header.size)
                   - sizeof(struct boundary_tag);
}

int main(void) {
	struct boundary_tag bt = {.inuse = 1, .size = 2};

	struct block b = {.header = bt};

	long b_start = (long)&b;
	long b_footer = (long)get_footer(&b);
	long b_end = (long)(get_footer(&b) + 1);

	printf("s: %u\n", b_start);
	printf("f: %u\n", b_footer);
	printf("e: %u\n", b_end);

	printf("f - s: %u\n", b_footer - b_start);
	printf("e - f: %u\n", b_end - b_footer);

	return 0;
}
