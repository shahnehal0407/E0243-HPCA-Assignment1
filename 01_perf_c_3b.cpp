#include<stdio.h>
#include<stdlib.h>

#define ROWS 8192
#define COLS 8192
#define SIZE 8192

#define BLOCK_SIZE 64

#define LIMIT 1000

void print_matrix(int** matrix, int N, int M){
    for(int row = 0; row < N; row++){
        for(int col = 0; col < M; col++){
            printf("%d ", matrix[row][col]);
        }
        printf("\n");
    }
}

int main(){

    // allocate memory dynamically to matrices A, B and C
    int **matrixA = (int**)malloc(ROWS * sizeof(int*));
    int **matrixB = (int**)malloc(ROWS * sizeof(int*));
    int **matrixC = (int**)malloc(ROWS * sizeof(int*));
    for(int row = 0; row < ROWS; row++){
        matrixA[row] = (int*)malloc(COLS * sizeof(int));
        matrixB[row] = (int*)malloc(COLS * sizeof(int));
        matrixC[row] = (int*)malloc(COLS * sizeof(int));
    }

    // initialize matrices A and B with some random values within 'LIMIT'
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            matrixA[row][col] = rand() % LIMIT;
            matrixB[row][col] = rand() % LIMIT;
            matrixC[row][col] = 0;
        }
    }


    if(matrixA == NULL || matrixB == NULL || matrixC == NULL){
        printf("Error: malloc() failed to allocate memory!\n");
        exit(0);
    }else{
        // printf("Success: memory allocated!\n");
    }

    // do matrix multiplication in a tiled fashion of tile-size=64
    // loop order: (j, i, k)
    for(int j = 0; j < COLS; j += BLOCK_SIZE){
        for(int i = 0; i < ROWS; i += BLOCK_SIZE){
            for(int k = 0; k < SIZE; k += BLOCK_SIZE){
                
                for(int jj = j; jj < j + BLOCK_SIZE; jj++){
                    for(int ii = i; ii < i + BLOCK_SIZE; ii++){
                        for(int kk = k; kk < k + BLOCK_SIZE; kk++){
                            matrixC[ii][jj] += (matrixA[ii][kk] * matrixB[kk][jj]);
                        }
                    }
                }

            }
        }
    }
    
    // printf("OK\n");

    // memory cleanup for malloced matrices A and B
    for(int row = 0; row < ROWS; row++){
        free(matrixA[row]);
        free(matrixB[row]);
    }
    free(matrixA);
    free(matrixB);

    return 0;
}
/*--------Instructions-------------*/
/*

Compile:
gcc 01_perf_c_3b.cpp -o 01_perf_c_3b
*/

/*
Run perf: perf stat -e duration_time,user_time,page-faults,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-load-misses,branch-misses,branch-instructions,bus-cycles ./01_perf_c_3b

*/
/*
 Performance counter stats for './01_perf_c_3b':

 1,692,886,438,544 ns   duration_time                                                         
 1,692,689,712,000 ns   user_time                                                             
           196,827      page-faults                                                           
12,149,808,482,668      L1-dcache-loads                                                         (43.75%)
    19,942,640,700      L1-dcache-load-misses            #    0.16% of all L1-dcache accesses   (43.75%)
 1,118,977,407,534      L1-dcache-stores                                                        (43.75%)
       151,370,928      L1-icache-load-misses                                                   (43.75%)
       517,173,439      LLC-loads                                                               (25.00%)
       404,442,395      LLC-load-misses                  #   78.20% of all L1-icache accesses   (25.00%)
        23,768,203      LLC-stores                                                              (12.50%)
        11,933,107      LLC-store-misses                                                        (12.50%)
12,151,335,842,639      dTLB-loads                                                              (18.75%)
       143,434,123      dTLB-load-misses                 #    0.00% of all dTLB cache accesses  (25.00%)
 1,118,934,040,459      dTLB-stores                                                             (25.00%)
         3,251,544      dTLB-store-misses                                                       (25.00%)
           206,662      iTLB-load-misses                                                        (25.00%)
     8,730,766,311      branch-misses                    #    1.51% of all branches             (31.25%)
   578,636,484,224      branch-instructions                                                     (37.50%)
    39,853,243,827      bus-cycles                                                              (43.75%)

    1692.886438544 seconds time elapsed

    1692.689712000 seconds user
       0.159998000 seconds sys



*/