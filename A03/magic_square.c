//---------------------------------------------------------------------
// magic_square.c 
// CS223 - Spring 2022
// Identify whether a matrix is a magic square
// Name: Linh Tran

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// return 1 if M is a magical matrix, 0 if not
int checkSum (int** M, int rows, int cols, int* constantSum) {
    int expectedSum = 0;

    // check rows
    for (int i = 0; i < rows; i++) {
        int currentSum = 0;
        for (int j = 0; j < cols; j++) {
            // sum of first row is the expected sum
            if (i == 0) { expectedSum += M[i][j]; }
            else { currentSum += M[i][j]; }
        }
        if (i >= 1 && currentSum != expectedSum){
            return 0;
        }
    }

    // check cols
    for (int j = 0; j < cols; j++) {
        int currentSum = 0;
        for (int i = 0; i < rows; i++) {
            currentSum += M[j][i];
        }
        if (currentSum != expectedSum){
            return 0;
        }
    }

    int currentSum = 0;
    // check the first diagonal (from left to right)
    for (int i = 0; i < rows; i++) {
        currentSum += M[i][i];
    }

    if (currentSum != expectedSum){
        return false;
    }

    currentSum = 0;
    // check the second diagonal (from right to left)
    int j = 0;
    for (int i = rows-1; i >= 0; i--) {
        currentSum += M[i][j++];
    }

    if (currentSum != expectedSum){
        return 0;
    }

    *constantSum = expectedSum;
    return 1;
}

int main(int argc, char *argv[]) {
    int nRows, nCols, k =0;
    scanf(" %d", &nRows);
    scanf(" %d", &nCols);

    int** matrix = malloc( sizeof(int*) * nRows);
    if (matrix == NULL) {
        printf("Cannot allocate new array. Exiting...\n");
        exit(1);
    }

    for (int i = 0; i < nRows; i++) {
        matrix[i] = malloc(sizeof(int) * nCols);
        if (matrix[i] == NULL) {
            printf("Cannot allocate new row. Exiting...\n");
            exit(1);
        }
    }

    // populate matrix
    for (int i = 0; i < nRows; i++) { 
        for (int j = 0; j < nCols; j++) { 
            scanf(" %d", &matrix[i][j]);
        } 
    }
    
    // print matrix
    for (int i = 0; i < nRows; i++) { 
        for (int j = 0; j < nCols; j++) { 
                printf("%d ", matrix[i][j]); 
        }
        printf("\n"); 
    }


    // check if matrix is magical
    int* constantSum = malloc(sizeof(int));
    
    if (constantSum == NULL) {
        printf("Cannot allocate new int. Exiting...\n");
        exit(1);
    }

    int ret = checkSum(matrix, nRows, nCols, constantSum);
    if(ret == 1) {
        printf("M is a magic square (magic constant = %d)\n", *constantSum); 
    } else {
        printf("M is NOT a magic square!\n");
    } 

    for (int i = 0; i < nRows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    matrix = NULL;

    free(constantSum);
    constantSum = NULL;
    return 0;
}

