#include <stdio.h>
#include <gmp.h>

int
main()
{
    mpf_t x;
    mpf_init(x);

    printf("sizeof(mpf_t) = %ld\n", sizeof(mpf_t));
    return 0;
}
