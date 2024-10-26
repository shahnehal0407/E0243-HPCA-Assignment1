#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <sys/mman.h>  // For mmap and related constants
#include <unistd.h>    // For sysconf and related functions
#include <stdio.h>     // For printf and perror
#include <stdlib.h>    // For exit

#define M_SIZE 8192

// void matrixMemoryAllocate(int*** ptr){
//     *ptr = (int**)malloc(M_SIZE*sizeof(int*));
//     for (int i=0; i< M_SIZE; i++) {
//         (*ptr)[i] = malloc(M_SIZE*sizeof(int));
//     }
// };

int** allocate_matrix() {
    // Allocate memory for a matrix using mmap
    int** matrix = (int**)mmap(NULL, M_SIZE * sizeof(int*) + M_SIZE * M_SIZE * sizeof(int),
                                PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB,
                                -1, 0);
    if (matrix == MAP_FAILED) {
        printf("Memory allocation failed using mmap");
        exit(1);
    }
 
    // Allocate each row of the matrix
    int* data = (int*)(matrix + M_SIZE);
    for (int i = 0; i < M_SIZE; ++i) {
        matrix[i] = data + (i * M_SIZE);
    }
    return matrix;
}


void init_matrix(int **matrix, int initVal) {
    for( int i = 0; i < M_SIZE; i++) {
        for( int j = 0; j < M_SIZE; j++) {
            matrix[i][j] = initVal;
        }
    }
};

void mul_matrix_ijk( int **matrix_A, int **matrix_B, int **matrix_res) {
    for(int i = 0 ; i < M_SIZE; i++) {
        for( int j = 0; j < M_SIZE; j++) {
            for(int k = 0; k < M_SIZE; k++) {
                matrix_res[i][j] += matrix_A[i][k]*matrix_B[k][j];
            }
        }
    }
};

void mul_matrix_jik( int **matrix_A, int **matrix_B, int **matrix_res) {
    for(int j = 0 ; j < M_SIZE; j++) {
        for( int i = 0; i < M_SIZE; i++) {
            for(int k = 0; k < M_SIZE; k++) {
                matrix_res[i][j] += matrix_A[i][k]*matrix_B[k][j];
            }
        }
    }
};

void mul_matrix_kij( int **matrix_A, int **matrix_B, int **matrix_res) {
    for(int k = 0 ; k < M_SIZE; k++) {
        for( int i = 0; i < M_SIZE; i++) {
            for(int j = 0; j < M_SIZE; j++) {
                matrix_res[i][j] += matrix_A[i][k]*matrix_B[k][j];
            }
        }
    }
};

void print(int **matrix_res) {
    for(int i = 0 ; i < M_SIZE; i++) {
        for( int j = 0; j < M_SIZE; j++) {
            printf("%d", matrix_res[i][j]);
        }
    }
};

int main() {

    int **matrix_A = allocate_matrix();
    int **matrix_B = allocate_matrix();
    int **matrix_AB = allocate_matrix();


    init_matrix(matrix_A, 3);
    init_matrix(matrix_B, 3);
    init_matrix(matrix_AB, 0);

    //mul_matrix_ijk(matrix_A, matrix_B, matrix_AB);
    //mul_matrix_jik(matrix_A, matrix_B, matrix_AB);
    mul_matrix_kij(matrix_A, matrix_B, matrix_AB);

    return 0;

}