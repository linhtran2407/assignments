#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

void char2bin (char c){
	int i, bin;
	char ch = c;
  int second, third;
  int bins[8];
  int index = 0;

	for (i = 0; i < 8; i++) {
		bin = ((ch << i) & 0x80) ? 1 : 0;
    if (i == 6) { second = bin; }
    if (i == 5) { third = bin; }
    bins[index++] = bin;
	}
  
  // swap
  bins[5] = second;
  bins[6] = third;  

  int dec = 0;
  int exp = 7;
  for (int i = 0; i < 8; i++, exp--) {
    if (bins[i] == 1) {
      dec += calcPower(2, exp);
    }
  }
  printf("%c", (char) dec);
}

int main() {
	char msg[1024];
  printf("Enter a phrase: \n");
  scanf("%[^\n]s", msg);
	char letter;
	printf("Scramble: ");
	for (int i = 0; i < strlen(msg); i++) {
		letter = msg[i];
		char2bin(letter);
	}

	printf("\n");
	return(0);
}