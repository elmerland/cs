/* 
 * CS:APP Data Lab 
 * 
 * Names: Elmer Landaverde, Harjas Ahuja
 * PIDs: elmer, harjas09
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
// selected puzzles for Spring 2013
/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 * Logic:
 *		A two’s compliment representation of -1 is equal to 1111 1111. Therefore to generate a -1, we just need to initialize a mask to zero (0000 0000) and the flip every bit by using the ~ (not) operator
 */
int minusOne(void) {
	int result = 0;
	result = ~result;
	return result;
}

/** 
 * oddBits - return word with all odd-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 *	Logic:
 *		First we initialize a mask that contains the first 8 odd bits turned in the on position (0xaa = 1010 1010). We then create a copy of that mask but with the bits shifter 8 places to the left and the OR it with the original mask. This takes care of the first 16 bits. We then do the same thing for the last 16 bits. We make a copy of the mask and move it 16 places to the left, and then we OR it with the original mask.
 */
int oddBits(void) {
	int mask = 0xaa;		// 0xaa = 1010 1010 all odd bits are one
	mask = mask | (mask << 8);
	mask = mask | (mask << 16);
	return mask;
}

/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 *	Logic:
 *		First we create a mask with only the first bit set to one (0x01). Then we AND the mask and the input. This will leave only the least significant bit. We then move that significant bit 31 spaces to the left which will place the LSB in the place occupied by the sign bit. No we just need to extend the sign bit 31 places to the right, and because it’s a signed integer it will copy what ever value is on the sign bit to the rest of the bits.
 */
int copyLSB(int x) {
	int mask = 0x01;
	mask = x & mask;
	mask = mask << 31;
	mask = mask >> 31;
	return mask;
}

/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 * Logic:
 *		First we create a mask to represent negative n (_n = -n). Then we create remainder mask with only the sign bit in the on position. Next we create a negative mask that has the same value as the remainder mask. We need to find two things: 1) if x has a remainder, 2) if x is negative. 
 
 To find out if x has a remainder we take the remainder mask and move it ‘31 – n’ to the left. This will create a mask that contains zero bits on the first ‘n’ bits. We then flip this usin ~ to get a mask that has ones on the first ‘n’ bits. Next we AND x and the remainder mask. If x/(2^n) has a remainder then when we AND x and the reminder mask we will get something greater than zero. If it doesn’t have a remainder then we get zero.  We then use !! on the final mask to get a 1 is there is a remainder and 0 if there is not a remainder.
 
 To find out if x is negative we first AND x and the negative mask. This will result in an empty number except for the sign bit.  We then use !! on the mask so that it is 1 if x is negative and 0 if x is not negative.
 
 Finally to output the result we compute x >> n. However, we need to offset this answer only when there is a remainder and x is negative. So we use the negative mask and the remainder mask. We AND these two mask, and we add the result. 
 */
int divpwr2(int x, int n) {
	int _n = ~n + 1;				// _n = -n
	int rem_mask = 0x80 << 24;		// remainder mask = 0x8000000 00000000
	int neg_mask = rem_mask;		// Negative mask = 0 if x>=0, 1 if x<0
	
		// Determine if x%(2^n) will have a remainder
	rem_mask = rem_mask >> (31 + _n);
	rem_mask = ~rem_mask;
	rem_mask = x & rem_mask;
	rem_mask = !!rem_mask;
	
		// Determine if x is negative.
	neg_mask = x & neg_mask;
	neg_mask = !!neg_mask;
	
	x = (x >> n) + (neg_mask & rem_mask);
    return x;
}

/* 
 * rempwr2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: rempwr2(15,2) = 3, rempwr2(-35,3) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 *	Logic:
 *		Fist we declare four masks. The first mask is equal to negative n (_n = -n). Then we declare a remainder mask that has only the sign bit turned on. Next we have a negative remainder mask, which has the ‘n’ bit turned on. Finally we declare a temporary mask that has every bit turned on, except for the ‘n’ bit.
 
 To determine if the remainder is a power of two we need to determine if x is negative. If x is negative then the remainder is equal to “-2^n + remainder”. 
 
 To determine if x is negative we just shift x 31 bits to the right, and store that in the negative mask. If x is negative then the negative mask is equal to -1, otherwise its equal to zero.
 
 Then we have to find the remainder. We shift the remainder mask “31 – n” bits to the right. This leaves the first n bits in the off position. Then we use ~ on the remainder mask which leaves the first n bits in the on position and the rest zero.
 
 Next we need to determine the remainder for when x is negative. We start by making the negative remainder mask equal to “2^n – remainder”.  
 Then we AND the negative remainder mask with the temporary mask. When the reminder is equal to zero then “2^n – 0 = 2^n”. If the remainder is equal to zero the answer must also be zero. So we use the temporary mask to set the n bit to zero, making the negative remainder mask equal to zero. We then take the negative remainder mask and turn it into a negative number, such that “- (2^n - remainder) = -2^n + remainder”. 
 
 Finally for the output, we use the negative mask to pick which mask will be returned. If x is negative the negative remainder will be returned. Otherwise the positive remainder will be returned. 
 */
int rempwr2(int x, int n) {
	int _n = ~n + 1;
	int rem_mask = 0x80 << 24;
	int _rem_mask = 0x01 << n;
	int t_mask = ~(_rem_mask);
	
		// Determine if x is negative.
	int neg_mask = x >> 31;
	
		// Determine mask for x >= 0
	rem_mask = rem_mask >> (31 + _n);
	rem_mask = ~rem_mask;
	rem_mask = x & rem_mask;	
	
		// Determine mask for x < 0
	 _rem_mask = _rem_mask + (~rem_mask + 1);
	 _rem_mask = _rem_mask & t_mask;
	 _rem_mask = ~_rem_mask + 1;
	
    return (rem_mask & ~neg_mask) + (_rem_mask & neg_mask);
}

/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 *	Logic:
 *		First we declare two masks. The first mask only has the sign bit turned on. The second mask is equal to zero.
 
 We first determine if the signs of x and y are equal. To do this we AND x with the sign mask and use “!”. This will result in 1 if x is positive, and 0 if x is negative. We do the same for y. We then XOR the results of the x and y signs and use “!”. This will return 1 if both signs are different and 0 if both signs are the same.
 
 Then we get the sign of the sum of x and y. For this, we add x and y, and we shift the result 31 bits to the right and use “!”. This will give us a 1 if the sum sign is positive, and 0 if the sum sign is negative.
 
 Finally we compute the return. This function should return false, only when an overflow occurs. We know that an overflow occurs when the x and y have the same sign, but the result contains a different sign. So we want to return false whenever the signs mask is true, and the XOR of the sign of x and the sum sign is true.
 
 */
int addOK(int x, int y) {
	int one_sign_mask = 0x01 << 31;
	int sum_sign_mask = 0x00;
	
	int x_sign_mask = !(x & one_sign_mask);
	int y_sign_mask = !(y & one_sign_mask);
	int signs = ~(x_sign_mask ^ y_sign_mask);
	
	sum_sign_mask = (x + y) >> 31;
	sum_sign_mask = !sum_sign_mask;
	
	return !(signs & (x_sign_mask ^ sum_sign_mask));
}

/*
 * ilog2 - return floor(log base 2 of x), where x > 0
 *   Example: ilog2(16) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 *	Logic:
 *		We start by creating four masks. The first mask has bits 31-16 turned on. The second mask has bits 15-9 turned on. The third mask has bits 8-5 turned on, and the fourth mask has bits 4-3 turned on. 
 
 To find the log base two of x, we need to know how many bits there are before the last 1 in the number. For example 001X XXXX, will have a log of 5 because there are five bits before the last 1.
 
 We first start by determining if there is any 1’s on the upper 16 bits. If this is the case then add 16 to output and shift x 16 places to the right.
 Next we determine if bits 15-9 have any 1’s. If this is the case, then add 8 to the output and shift x 8 places to the right. 
 Next we determine if bits 8-5 have any 1’s. If this is the case, then add 4 to the output and shift x 4 places to the right.
 Next we determine if bits 4-3 have any 1’s. If this is the case, then add 2 to the output and shift x 2 places to the right.
 Finally, shift x 1 place to the right, and add the result to the output
 
 Return the output.
 
 The logic behind the following: you start with the upper half. Every time you find that there is a 1 in one of the upper half bits you discard the other half and do the same with the remaining half. If however, there are not 1’s in the upper half then you ignore the upper half, and do the same with the lower half.
 
 */
int ilog2(int x) {
    int mask_high_16 = (0x01 << 31) >> 15;
    int mask_high_8 = 0xFF << 8;
    int mask_high_4 = 0xF0;
    int mask_high_2 = 0x0C;
    int log = 0;
    int shift = 0;
	
		// Check bit 31-17
    shift = !!(x & mask_high_16) << 4;
    x = x >> shift;
	log = log + shift;
	
		// Check bit 16-9
    shift = !!(x & mask_high_8) << 3;
    x = x >> shift;
    log = log + shift;
	
		// Check bit 8-5
    shift = !!(x & mask_high_4) << 2;
    x = x >> shift;
    log = log + shift;
	
		// Check bit 4-3
    shift = !!(x & mask_high_2) << 1;
    x = x >> shift;
    log = log + shift;
	
		// Check bit 2
	shift = x >> 1;
	log = log + shift;
	
    return log;
}

/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 *	Logic:
 *		First we create two masks, a negative one mask, then a x-1 mask.
 
 We first start by AND x and x-1 mask. If x is a power of 2, then the result of this should be zero. Otherwise it will return 1.
 
 Then we determine if x is negative. We do this by shifting x 31 bits to the right and using “!”. This will return 1 if x is positive and 0 if x is negative.
 
 Lastly we return the output of the function. This should function should return true only when x is not negative, x is not zero, and x is a power of two. So we just AND these three masks together and return the result.
 
 */
int isPower2(int x) {
	int neg_one = ~0x00;
	int low_ones = x + neg_one;
	
	int result = x & low_ones;
	
	int neg_mask = x >> 31;
	
	return !result & !neg_mask & !!x;
}