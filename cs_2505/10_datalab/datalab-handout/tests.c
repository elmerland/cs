/* Testing Code */

#include <limits.h>
#include <math.h>

/* Routines used by floation point test code */

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
  union {
    unsigned u;
    float f;
  } a;
  a.u = u;
  return a.f;
}

/* Convert from floating point number to bit-level representation */
unsigned f2u(float f) {
  union {
    unsigned u;
    float f;
  } a;
  a.f = f;
  return a.u;
}

// selected puzzles for Spring 2013
int test_minusOne(void) {
  return -1;
}
int test_oddBits(void) {
  int result = 0;
  int i;
  for (i = 1; i < 32; i+=2)
    result |= 1<<i;
  return result;
}
int test_copyLSB(int x)
{
  return (x & 0x1) ? -1 : 0;
}
int test_divpwr2(int x, int n)
{
    int p2n = 1<<n;
    return x/p2n;
}
int test_rempwr2(int x, int n)
{
    int p2n = 1<<n;
    return x%p2n;
}







int test_addOK(int x, int y)
{
    long long lsum = (long long) x + y;
    return lsum == (int) lsum;
}
int test_ilog2(int x) {
  int mask, result;
  /* find the leftmost bit */
  result = 31;
  mask = 1 << result;
  while (!(x & mask)) {
    result--;
    mask = 1 << result;
  }
  return result;
}
int test_isPower2(int x) {
  int i;
  for (i = 0; i < 31; i++) {
    if (x == 1<<i)
      return 1;
  }
  return 0;
}
