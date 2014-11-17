/*
 * Accessing locations outside the allocated area. (2)
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int
main()
{
    char * a = calloc(16, 1);
    char * b = calloc(16, 1);

    a[-15] = '1';
    a[-14] = '1';
    a[-13] = '1';
    a[-12] = '1';
    a[-11] = '1';
    a[-10] = '1';
    a[-9] = '1';
    a[-8] = '1';
    a[-7] = '1';
    a[-6] = '1';
    a[-5] = '1';
    a[-4] = '1';
    a[-3] = '1';
    a[-2] = '1';
    a[-1] = '1';
    a[200] = '1';        // out of bounds
    b[-200] = '2';       // out of bounds

    printf("a=%c b=%c\n", *a, *b);
    return 0;
}
