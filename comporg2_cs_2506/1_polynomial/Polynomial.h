/*
 * Polynomial.h
 *
 *  Every properly initialized Polynomial object P is required to satisfy
 *  the following polynomial contract:
 *
 *    1.  P.Coeff points to an array of dimension P.Degree + 1.
 *    2.  P.Coeff[k] is the coefficient of x^k , for k = 0 to P.Degree.
 */

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <stdint.h>
#include <stdbool.h>

struct Poly {
	uint8_t  Degree;  // highest power of x in Polynomial
	int64_t *Coeff;   // array of Degree + 1 cofficients; 
                     //   Coeff[k] is coefficient of x^k, k = 0 to Degree
};

typedef struct Poly Polynomial;

/**
 * Initializes *P as described below.
 *
 * Pre:  P points to an uninitialized Polynomial object,
 *       C != NULL,
 *       C[i] initialized for i = 0:D
 * Post: P->Degree == D,
 *       P->Coeff != C (array is duplicated, not linked),
 *       P->Coeff[i] == C[i] for i = 0:D
 * Returns: false if *P cannot be properly initialized, true otherwise
 */
bool    Polynomial_Set(Polynomial* const P, const uint8_t D, const int64_t* const C);

/**
 * Initializes *Target from *Source as described below.
 *
 * Pre:  Target points to a Polynomial object,
 *       Source points to a properly-initialized Polynomial object
 * Post: Target->Degree == Source->Degree,
 *       Target->Coeff != Source->Coeff,
 *       Target->Coeff[i] == Source->Coeff[i] for i = 0:Source->Degree
 * Returns: false if *Target cannot be properly initialized, true otherwise
 */
bool    Polynomial_Copy(Polynomial* const Target, const Polynomial* const Source);

/**
 * Compares two polynomials.
 *
 * Pre:  Left points to a properly-initialized Polynomial object,
 *       Right points to a properly-initialized Polynomial object
 * Returns: true if Left and Right have the same coefficients, false otherwise
 */
bool    Polynomial_Equals(const Polynomial* const Left, const Polynomial* const Right);

/**
 * Computes value of polynomial at X.
 *
 * Pre:  P points to a properly-initialized Polynomial object
 * Returns: value of P(X); 0 if cannot be evaluated
 */
int64_t Polynomial_EvaluateAt(const Polynomial* const P, const int64_t X);

/**
 * Initializes *Scaled to represent K times *Source
 *
 * Pre:  Scaled points to a Polynomial object,
 *       Source points to a properly-initialized Polynomial object,
 *       Source != Target
 * Post: Scaled->Degree == Source->Degree,
 *       Scaled->Coeff  != Source->Coeff,
 *       Scaled->Coeff[i] == K * Source->Coeff[i] for i = 0:Scaled->Degree
 * Returns: false if *Scaled cannot be properly initialized, true otherwise
 */
bool    Polynomial_Scale(Polynomial* const Scaled, const Polynomial* const Source, const int64_t K);

/**
 * Initializes *Sum to equal *Left + *Right.
 *
 * Pre:  Sum points to a Polynomial object,
 *       Left points to a properly-initialized Polynomial object,
 *       Right points to a properly-initialized Polynomial object,
 *       Sum != Left,
 *       Sum != Right
 * Post: Sum->Degree == max(Left->Degree, Right->Degree),
 *       Sum->Coeff[i] == Left->Coeff[i] + Right->Coeff[i] 
 *           with proper allowance for the case that 
 *           Left->Degree != Right->Degree
 * Returns: false if *Sum cannot be properly initialized, true otherwise
 */
bool    Polynomial_Add(Polynomial* const Sum, const Polynomial* const Left, const Polynomial* const Right);

/**
 * Initializes *Diff to equal *Left - *Right.
 *
 * Pre:  Diff points to a Polynomial object,
 *       Left points to a properly-initialized Polynomial object,
 *       Right points to a properly-initialized Polynomial object,
 *       Diff != Left,
 *       Diff != Right
 * Post: Diff->Degree is set correctly,
 *       Diff->Coeff[i] == Left->Coeff[i] - Right->Coeff[i] 
 *           with proper allowance for the case that 
 *           Left->Degree != Right->Degree
 * Returns: false if *Diff cannot be properly initialized, true otherwise
 */
bool    Polynomial_Subtract(Polynomial* const Diff, const Polynomial* const Left, const Polynomial* const Right);

/**
 * Computes the first derivative of Source.
 *
 * Pre:  Target points to a Polynomial object,
 *       Source points to a properly-initialized Polynomial object,
 *       Target != Source
 * Post: Target->Degree is set correctly
 *       Target->Coeff[i] == iith coefficient of Source'
 *
 * Returns: false if Source' cannot be properly initialized, true otherwise
 */
bool    Polynomial_Differentiate(Polynomial* const Target, const Polynomial* const Source);

/**
 * Reset P to represent zero polynomial.
 *
 * Pre:  P points to a Polynomial object
 * Post: P->Degree == 0
 *       P->Coeff is set appropriately
 */
bool    Polynomial_Zero(Polynomial* const P);

#endif /* POLYNOMIAL_H_ */
