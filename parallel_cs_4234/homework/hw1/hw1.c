#include <stdio.h>

int main(void) {
  int n = 20;
  int p = 4;
  for (int i = 0; i < 10; i++) {
    int r = n % p;
    printf("n: %d, p: %d, r: %d\n", n, p, r);
    for (int p_id = 0; p_id < p; p_id++) {
      int offset = 0;
      int step = 0;
      int idx = 0;
      if (p_id < r) {
        offset = 0;
        step = n/p + 1;
        idx = p_id;
      } else {
        offset = ((n/p) + 1) * r;
        step = n/p;
        idx = p_id - r;
      }
      int my_fist_i = offset + (step * idx);
      int my_last_i = my_fist_i + step;
      int num = my_last_i - my_fist_i;
      printf("P: %d\t first: %d, last: %d\t\tnum: %d, off: %d, step: %d\n", p_id, my_fist_i, my_last_i, num, offset, step);
    }
    n += 3;
  }
  return 0;
}