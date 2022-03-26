#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	char msg[1024];
  printf("Enter a phrase: \n");
  scanf("%[^\n]s", msg);
	
	printf("Scramble: ");
	for (int i = 0; i < strlen(msg); i++) {
		unsigned int secondBit = msg[i] & 0b00000010;
		unsigned int thirdBit = msg[i] & 0b00000100;
    

    if (secondBit == 0b00000000 && thirdBit == 0b00000100){
      msg[i] = msg[i] | 0b00000010; // set second bit to 1
      msg[i] = msg[i] & 0b11111011; // set third bit to 0
    } else if (secondBit == 0b00000010 && thirdBit == 0b00000000){
      msg[i] = msg[i] & 0b11111101; // set second bit to 0
      msg[i] = msg[i] | 0b00000100; // set third bit to 1
    }

    printf("%c", msg[i]);
	}

	printf("\n");
	return(0);
}