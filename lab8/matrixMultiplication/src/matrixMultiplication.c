#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

#define LOOPBYROW       0
#define LOOPBYCOLUMN    1

int** matrixMultiplication(int** m1, int height1, int width1, int** m2, int height2, int width2, int loopArrangement){
    // Check to see that the width of the first matrix is equal to the 
    // height of the second matrix. Otherwise, they cannot be multiplied;
    if(width1 != height2)
        return NULL;

    // Create a result matrix of size height1 x width2
    // Create outer array of double pointers
    int** result = (int**)malloc(height1 * sizeof(int*));

    // Create inner array of single pointers
    for(int i = 0; i < height1; i++)
        result[i] = (int*)malloc(width2 * sizeof(int));

    if(loopArrangement == LOOPBYROW){
        printf("Looping by row...\n");
        for(int i = 0; i < height1; i++){
            for(int j = 0; j < width2; j++){
                int tally = 0;
                for(int k = 0; k < width1; k++){
                    tally += (m1[i][k] * m2[k][j]);
                }
                result[i][j] = tally;
            }
        }
    } else if (loopArrangement == LOOPBYCOLUMN){
        printf("Looping by column...\n");
        for(int i = 0; i < width2; i++){
            for(int j = 0; j < height1; j++){
                int tally = 0;
                for(int k = 0; k < width1; k++){
                    tally += (m2[k][i] * m1[j][k]);
                }
                result[j][i] = tally;
            }
        }
    }

    return result;
}

int printMatrix(const char* name, int** mat, int height, int width){
    printf("%s:\n", name);
    for(int i = 0; i < height; i++){
        printf("\t");
        for(int j = 0; j < width; j++){   
            printf("%-4d", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}

int freeMatrix(int** mat, int height, int width){
    for(int i = 0; i < height; i++)
        free(mat[i]);
    
    free(mat);

    return 0;
}

int main(int argc, char* argv[]){

    int gridSize = 5;

    // Create first matrix: 1 2 3
    //                      4 5 6
    //                      7 8 9
    int** mat1 = (int**)malloc(gridSize * sizeof(int*));
    for(int i = 0; i < gridSize; i++)
        mat1[i] = (int*)malloc(gridSize * sizeof(int));
    
    for(int i = 0; i < gridSize; i++)
        for(int j = 0; j < gridSize; j++)
            mat1[i][j] = i;

    // Create second matrix: 1 2
    //                       3 4
    //                       5 6
    int** mat2 = (int**)malloc(gridSize * sizeof(int*));
    for(int i = 0; i < gridSize; i++)
        mat2[i] = (int*)malloc(gridSize * sizeof(int));
    
    for(int i = 0; i < gridSize; i++)
        for(int j = 0; j < gridSize; j++)
            mat2[i][j] = j;

    // Test print
    printMatrix("Matrix 1", mat1, gridSize, gridSize);
    printMatrix("Matrix 2", mat2, gridSize, gridSize);

    int loopArrangement;
    if(argc < 2 || (strcmp(argv[1], "row") == 0))
        loopArrangement = LOOPBYROW;
    else
        loopArrangement = LOOPBYCOLUMN;


    int** result = matrixMultiplication(mat1, gridSize, gridSize, mat2, gridSize, gridSize, loopArrangement);
    printMatrix("Result matrix", result, gridSize, gridSize);

    // Free matrices
    freeMatrix(mat1, gridSize, gridSize);
    freeMatrix(mat2, gridSize, gridSize);
    freeMatrix(result, gridSize, gridSize);

    return 0;
}