#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printBorder(int len, char sym) {
    for (int i = 0; i < (len+4); i++) {
        printf("%c", sym);
    }
    printf("\n");
}
int main() {
    char s[64];
    char sym;

    printf("Enter a word: ");
    scanf(" %s", s);
    printf("Enter a symbol: ");
    scanf(" %c", &sym);

    int len = strlen(s);

    printBorder(len, sym); 

    printf("%c %s %c\n", sym, s, sym);

    printBorder(len, sym); 
    return 0;
}