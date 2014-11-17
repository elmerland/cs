void * Pth_mat_vect(void * rank) {
  long my_rank = (long) rank;
  int i, j;
  int my_first_row, my_last_row;
  get_offset(my_rank, thread_count, m, 
    &my_first_row, &my_last_row);

  for (i = my_first_row; i < my_last_row; i++) {
    y[i] = 0.0;
    for (j = 0; j < n; j++) {
      y[i] += A[i][j] * x[j];
    }
  }
  return NULL;
} /* Pth_mat_vect */

void get_offset( int rank,    /* Input */
                 int threads, /* Input */
                 int rows,    /* Input */
                 int * start, /* Output */
                 int * end    /* Output */ ) {
  int r = rows % threads;
  int offset, step, idx;
  if (rank < r) {
    offset = 0;
    step = (rows / threads) + 1;
    idx = rank;
  } else {
    offset = ((rows / threads) + 1) * r;
    step = rows / threads;
    idx = rank - r;
  }
  *start = offset + (step * idx);
  *end = (*start) + step;
} /* get_offset */