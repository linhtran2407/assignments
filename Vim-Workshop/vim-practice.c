/*
 * Linh Tran
 */

#include <stdio.h>
#include <string.h>

int main() {
	printf("Hello vim\n");
	int a = 0;
	printf("Enter a number: ");
  scanf("%i", &a);
  
	if (a < 0) {
  		printf("a less than zero\n");
	} else if (a > 0) {
  		printf("a greater than zero\n");
	} else {
  		printf("you win!\n");
	}

	return 0;
}






















































