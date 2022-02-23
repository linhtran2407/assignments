#include <stdio.h>
#include <math.h>
#include<stdlib.h>

// avoid using math.h because that requires
// linking to math library when compile
int calcPower (int base, int exp) {
  int res = 1;
  while (exp > 0) {
    res *= base;
    --exp;
  }
  
  return res;
}

int main() {
  char digits[9];
  printf("Enter 8 bits: \n");
  scanf(" %s", digits);

  // calc decimal value of binary number
  int exp = 7;
  int decVal = 0;
  for (int i = 0; i < 8; i++, exp--) {
    if (digits[i] == '1') {
      decVal += calcPower(2, exp);
    }
  }

  printf("Your character is: %c\n", (char) decVal);
  return 0;
}
