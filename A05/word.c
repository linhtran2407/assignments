#include <stdio.h>

int main() {
  char chars[5];
  printf("Enter 4 characters: \n");
  scanf(" %s", chars);

  unsigned int shiftedThirdChar = chars[2] << 8;
  unsigned int shiftedSecondChar = chars[1] << 16;
  unsigned int shiftedFirstChar = chars[0] << 24; 
  printf("Debug: %c = 0x%08X\n", chars[3], chars[3]); // unchange
  printf("Debug: %c = 0x%08X\n", chars[2], shiftedThirdChar);
  printf("Debug: %c = 0x%08X\n", chars[1], shiftedSecondChar);
  printf("Debug: %c = 0x%08X\n", chars[0], shiftedFirstChar);
  unsigned int sum = (unsigned int) chars[3] + shiftedFirstChar + shiftedSecondChar + shiftedThirdChar;

  printf("Your number is: %d (0x%08x)\n", sum, sum);

  return 0;
}
