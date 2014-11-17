#ifndef ADDEM_H
#define ADDEM_H

/**
 *   Computes and returns sum of A[0]:A{Sz-1].
 *   Pre:
 *         A points to an array of dimension at least Sz
 *         A[0:Sz-1] are initialized
 *   Returns:
 *         sum of A[0] through A[Sz-1]
 */
int AddEm(const int* A, int Sz);

#endif

