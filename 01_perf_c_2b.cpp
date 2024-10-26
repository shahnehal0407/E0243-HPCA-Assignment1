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
    // loop order: (k, i, j)
    for(int k = 0; k < SIZE; k += BLOCK_SIZE){
        for(int i = 0; i < ROWS; i += BLOCK_SIZE){
            for(int j = 0; j < COLS; j += BLOCK_SIZE){
                
                for(int kk = k; kk < k + BLOCK_SIZE; kk++){
                    for(int ii = i; ii < i + BLOCK_SIZE; ii++){
                        for(int jj = j; jj < j + BLOCK_SIZE; jj++){
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
gcc 01_perf_c_2b.cpp -o 01_perf_c_2b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_2b

*/
/*
Performance counter stats for './01_perf_c_2b':

 1,579,586,545,771 ns   duration_time                                                         
 1,579,331,811,000 ns   user_time                                                             
           196,808      page-faults                                                           
12,149,874,528,789      L1-dcache-loads                                                         (43.75%)
     3,672,203,829      L1-dcache-load-misses            #    0.03% of all L1-dcache accesses   (43.75%)
 1,118,889,954,245      L1-dcache-stores                                                        (43.75%)
       144,493,229      L1-icache-load-misses                                                   (43.75%)
        83,369,550      LLC-loads                                                               (25.00%)
        29,758,027      LLC-load-misses                  #   35.69% of all L1-icache accesses   (25.00%)
        21,997,877      LLC-stores                                                              (12.50%)
        11,181,211      LLC-store-misses                                                        (12.50%)
12,153,672,025,760      dTLB-loads                                                              (18.75%)
         3,635,874      dTLB-load-misses                 #    0.00% of all dTLB cache accesses  (25.00%)
 1,118,969,231,880      dTLB-stores                                                             (25.00%)
         3,224,840      dTLB-store-misses                                                       (25.00%)
           116,615      iTLB-load-misses                                                        (25.00%)
     8,717,341,208      branch-misses                    #    1.51% of all branches             (31.25%)
   578,559,572,924      branch-instructions                                                     (37.50%)
    37,105,852,844      bus-cycles                                                              (43.75%)

    1579.586545771 seconds time elapsed

    1579.331811000 seconds user
       0.187996000 seconds sys
*/