#include <stdio.h>
#include <math.h>
#include<stdlib.h>

int main() {
  char digits[9];
  printf("Enter 8 bits: \n");
  scanf(" %s", digits);

  // calc decimal value of binary number
  int exp = 7;
  int decVal = 0;
  for (int i = 0; i < 8; i++, exp--) {
    if (digits[i] == '1') {
      decVal += pow(2, exp);
    }
  }

  printf("Your character is: %c\n", (char) decVal);
  return 0;
}
