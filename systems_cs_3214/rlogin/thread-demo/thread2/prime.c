#include "prime.h"


bool is_prime(int num) {
  
  if (num%2 == 0) {
    return false;
  }
  int i = 3;
  bool divisible = false;

  while((i < sqrt(num)) && 
        (!divisible )) {
    if (num%i == 0) divisible = true;
    else i++;
  }

  if(divisible) return false;
  else          return true;;
}

