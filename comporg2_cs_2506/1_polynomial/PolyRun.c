#include "Polynomial.h"
#include <inttypes.h>
#include <stdio.h>

void printPoly(Polynomial* const p);

int main() {
	
	int64_t a[5] = {1, 2, 3, 4, 5};
		//int64_t a2[5] = {1, 2, 3, 4, -5};
		//int64_t a3[6] = {1, 2, 3, 4, -5, 8};
	uint8_t d = 4;
	Polynomial p1;
	p1.Coeff = NULL;
	Polynomial p2;
	p2.Coeff = NULL;
	Polynomial p3;
	p3.Coeff = NULL;
	Polynomial p4;
	p4.Coeff = NULL;
	Polynomial p5;
	p5.Coeff = NULL;
	Polynomial p6;
	p6.Coeff = NULL;
	
	/*Polynomial_Set(&p1, d, a1);
	Polynomial_Set(&p2, d, a2);
	Polynomial_Set(&p3, d+1, a3);
	
	Polynomial_Add(&p4, &p1, &p2);
	printPoly(&p4);
	Polynomial_Add(&p5, &p1, &p3);
	printPoly(&p5);
	Polynomial_Subtract(&p6, &p2, &p1);
	printPoly(&p6);*/
	
	
	if (Polynomial_Set(&p1, d, a)) {
		printf("Polynomial set = true\n");
	}
	
	
	if (Polynomial_Copy(&p2, &p1)) {
		printf("Polynomial copy = true\n");
	}
	
	printPoly(&p1);
	printPoly(&p2);
	
	if (Polynomial_Equals(&p1, &p2)) {
		printf("Polynomial equals = true\n");
	}
	
	int64_t result = Polynomial_EvaluateAt(&p1, 10);
	printPoly(&p1);
	printf("Result:\t%"PRId64"\n", result);
	if (result != 0){
		printf("Polynomial evaluate = true\n");
	}

	if (Polynomial_Scale(&p3, &p1, 2)) {
		printf("Polynomial scale = true\n");
	}
	
	printPoly(&p3);
	
	if (Polynomial_Add(&p4, &p1, &p3)) {
		printf("Polynomial sum = true\n");
	}
	
	printPoly(&p1);
	printPoly(&p3);
	printPoly(&p4);
	
	/*if (Polynomial_Subtract(&p5, &p4, &p1)) {
		printf("Polynomial diff = true\n");
	}
	
	printPoly(&p5);*/
	
	if (Polynomial_Differentiate(&p6, &p1)) {
		printf("Polynomial diff = true\n");
	}
	
	printPoly(&p6);
	
	if (Polynomial_Zero(&p1)) {
		printf("Polynomial zero = true\n");
	}
	
	printPoly(&p1);
	return 1;
}

void printPoly(Polynomial* const p) {
	printf("Degree:\t%d\n", p->Degree);
	printf("Coeffs:\t");
	for (int i = 0; i <= p->Degree; i++) {
		if (i != 0) {
			printf("\t");
		}
		printf("[%d]\t<%"PRId64">\n", i, p->Coeff[i]);
	}
}