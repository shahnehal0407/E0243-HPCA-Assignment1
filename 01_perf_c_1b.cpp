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
    // loop order: (i, j, k)
    for(int i = 0; i < ROWS; i += BLOCK_SIZE){
            for(int j = 0; j < COLS; j += BLOCK_SIZE){
                for(int k = 0; k < SIZE; k += BLOCK_SIZE){
                
                    for(int ii = i; ii < i + BLOCK_SIZE; ii++){
                        for(int jj = j; jj < j + BLOCK_SIZE; jj++){
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
gcc 01_perf_c_1b.cpp -o 01_perf_c_1b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_1b

*/
/*

 Performance counter stats for './01_perf_c_1b':

 1,756,026,040,607 ns   duration_time                                                         
 1,755,841,046,000 ns   user_time                                                             
           196,805      page-faults                                                           
12,150,517,050,774      L1-dcache-loads                                                         (43.75%)
     3,446,557,830      L1-dcache-load-misses            #    0.03% of all L1-dcache accesses   (43.75%)
 1,118,981,561,228      L1-dcache-stores                                                        (43.75%)
       149,885,643      L1-icache-load-misses                                                   (43.75%)
       371,554,980      LLC-loads                                                               (25.00%)
       283,490,200      LLC-load-misses                  #   76.30% of all L1-icache accesses   (25.00%)
        24,699,157      LLC-stores                                                              (12.50%)
        11,171,659      LLC-store-misses                                                        (12.50%)
12,151,094,554,189      dTLB-loads                                                              (18.75%)
       143,697,521      dTLB-load-misses                 #    0.00% of all dTLB cache accesses  (25.00%)
 1,118,892,390,497      dTLB-stores                                                             (25.00%)
         3,263,733      dTLB-store-misses                                                       (25.00%)
           185,199      iTLB-load-misses                                                        (25.00%)
     8,732,335,847      branch-misses                    #    1.51% of all branches             (31.25%)
   578,749,548,881      branch-instructions                                                     (37.50%)
    41,737,917,678      bus-cycles                                                              (43.75%)

    1756.026040607 seconds time elapsed

    1755.841046000 seconds user
       0.139997000 seconds sys




*/