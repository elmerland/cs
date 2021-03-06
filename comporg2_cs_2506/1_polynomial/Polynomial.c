/*
 * Polynomial.h
 *
 *  Every properly initialized Polynomial object P is required to satisfy
 *  the following polynomial contract:
 *
 *    1.  P.Coeff points to an array of dimension P.Degree + 1.
 *    2.  P.Coeff[k] is the coefficient of x^k , for k = 0 to P.Degree.
 */
#include "Polynomial.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

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
bool    Polynomial_Set(Polynomial* const P, const uint8_t D, const int64_t* const C){
        if (P == NULL || C == NULL) 
		{
			return false;
		}
        P->Degree = D;
		if (P->Coeff != NULL)
		{
			free(P->Coeff);
		}
		P->Coeff = malloc((D + 1) * sizeof(int64_t));
		if (P != NULL)
		{
			for (int i = 0; i <= D; i++)
			{
				P->Coeff[i] = C[i];
			}
		return true;
		}
		else{
		return false;
		}

}

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
bool    Polynomial_Copy(Polynomial* const Target, const Polynomial* const Source){
		if (Target == NULL || Source == NULL)
		{
			return false;
		}
		Target->Degree = Source->Degree;
		Target->Coeff = malloc((Target->Degree) * sizeof(int64_t));
		for (int i = 0; i <= Source->Degree; i++)
		{
			Target->Coeff[i] = Source->Coeff[i];
		}
		return true;
}

/**
 * Compares two polynomials.
 *
 * Pre:  Left points to a properly-initialized Polynomial object,
 *       Right points to a properly-initialized Polynomial object
 * Returns: true if Left and Right have the same coefficients, false otherwise
 */
bool    Polynomial_Equals(const Polynomial* const Left, const Polynomial* const Right){
		if (Left->Degree != Right->Degree)
		{
			return false;
		}
		for (int i = 0; i <= Left->Degree; i++)
		{
			if (Left->Coeff[i] == Right->Coeff[i])
			{
				return true;
			}
		}
		return false;
}

/**
 * Computes value of polynomial at X.
 *
 * Pre:  P points to a properly-initialized Polynomial object
 * Returns: value of P(X); 0 if cannot be evaluated
 */
int64_t Polynomial_EvaluateAt(const Polynomial* const P, const int64_t X){
		if (P == NULL)
		{
			return 0;
		}
		int64_t power = X;
		int64_t solution = P->Coeff[0];
		for (int i = 1; i <= P->Degree; i++)
		{
			solution = solution + (P->Coeff[i] * power);
			power = power * X;
		}
		return solution;
}

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
bool    Polynomial_Scale(Polynomial* const Scaled, const Polynomial* const Source, const int64_t K){
		if (Scaled == NULL || Source == NULL)
		{
			return false;
		}
		Polynomial_Copy(Scaled, Source);
		if (Scaled == NULL) {
			return false;
		}
		else{
			
		for (int i = 0; i <= Scaled->Degree; i++)
		{
			Scaled->Coeff[i] = K * Scaled->Coeff[i];
		}
		return true;
		}
}

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
bool    Polynomial_Add(Polynomial* const Sum, const Polynomial* const Left, const Polynomial* const Right){
		if (Left == NULL || Right == NULL || Sum == NULL)
		{
			return false;
		}
		if (Right->Degree > Left->Degree)
		{
				
				if (Polynomial_Set(Sum, Right->Degree, Right->Coeff))
				{
					for (int i = 0; i <= Left->Degree; i++)
					{
						Sum->Coeff[i] = Sum->Coeff[i] + Left->Coeff[i];
					}
				}
				else{
				return false;
				}
		}
		else
		{
				if (Polynomial_Set(Sum, Left->Degree, Left->Coeff))
				{
					for (int i = 0; i <= Right->Degree; i++)
					{
						Sum->Coeff[i] = Sum->Coeff[i] + Right->Coeff[i];
					}
				}
				else{
				return false;}
		}
		for (int i = Sum->Degree; i >= 0; i--)
		{
			int64_t temp = Sum->Coeff[i];
			if (temp == 0 && temp == Sum->Degree)
			{
				Sum->Degree = Sum->Degree - 1;
			}
		}
		Polynomial_Set(Sum, Sum->Degree, Sum->Coeff);
		return true;
		
		
}

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
bool    Polynomial_Subtract(Polynomial* const Diff, const Polynomial* const Left, const Polynomial* const Right) {
	return false;
}

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
bool    Polynomial_Differentiate(Polynomial* const Target, const Polynomial* const Source){

		if (Target == NULL || Source == NULL)
		{
			return false;
		}
		if (Source->Degree == 0)
		{
			Polynomial_Zero(Target);
			return true;
		}
		//Assigning target degree and coefficient
		Target->Degree = Source->Degree - 1;
		Target->Coeff = malloc((Source->Degree) * sizeof(int64_t));
		for (int i = 1; i <= Source->Degree; i++)
		{
			Target->Coeff[i - 1] = i * Source->Coeff[i];
		}
		return true;
}

/**
 * Reset P to represent zero polynomial.
 *
 * Pre:  P points to a Polynomial object
 * Post: P->Degree == 0
 *       P->Coeff is set appropriately
 */
bool    Polynomial_Zero(Polynomial* const P){
		if (P != NULL)
		{
			P->Degree = 0;
			if (P->Coeff != NULL){
			free(P->Coeff);
			}
			P->Coeff = malloc((P->Degree + 1) + sizeof(int64_t));
			P->Coeff[0] = 0;
			
			return true;
		}
		else{
		return false;
		}
}
