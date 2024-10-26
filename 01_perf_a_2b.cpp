/*
Author: akashmaji(@iisc.ac.in)
File:   01_perf_a_2.cpp
*/

#include<stdio.h>
#include<stdlib.h>

#define ROWS 8192
#define COLS 8192
#define SIZE 8192

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
        }
    }

    // initialize A, B to zeros
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
             matrixC[i][j] = 0;
        }
    }
    
    // do the matrix multiplication C = A * B
    // loop order is (j, i, k)
    for(int j = 0; j < COLS; j++){
        for(int i = 0; i < ROWS; i++){
            for(int k = 0; k < SIZE; k++){
                matrixC[i][j] += (matrixA[i][k] * matrixB[k][j]);
            }
        }
    }

    // see matrix C
    // print_matrix(matrixC, ROWS, COLS);

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
gcc 01_perf_a_2b.cpp -o 01_perf_a_2b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_2b

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_2b

 Performance counter stats for './01_perf_a_2b':

 2,523,342,901,674 ns   duration_time                                                         
 2,519,410,180,000 ns   user_time                                                             
       383,989,000 ns   system_time                                                           
 2,063,493,194,276      cache-references                                                        (41.67%)
 1,140,332,956,041      cache-misses                     #   55.26% of all cache refs           (41.67%)
   664,088,588,789      L1-dcache-load-misses                                                   (41.67%)
        77,493,379      L1-icache-load-misses                                                   (41.67%)
   553,907,097,987      dTLB-load-misses                                                        (41.67%)
   555,770,318,672      l1_dtlb_misses                                                          (41.67%)
   553,990,891,431      l2_dtlb_misses                                                          (41.67%)
26,982,395,266,607      instructions                     #    2.80  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
 9,628,180,128,763      cpu-cycles                                                              (41.67%)
   125,028,029,509      stalled-cycles-frontend          #    1.30% frontend cycles idle        (41.67%)
   557,076,258,602      branch-instructions                                                     (41.67%)
       739,121,534      branch-misses                    #    0.13% of all branches             (41.67%)
           196,806      page-faults                                                           

    2523.342901674 seconds time elapsed

    2519.410180000 seconds user
       0.383989000 seconds sys


akash@victus:~/Desktop/HPCA$

*/
/*-------------------------*/

/*
Run perf:perf stat -e duration_time,user_time,page-faults,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-load-misses,branch-misses,branch-instructions,bus-cycles ./01_perf_a_2b

*/
/*

Performance counter stats for './01_perf_a_2b':

 4,426,857,946,943 ns   duration_time                                                         
 4,425,380,384,000 ns   user_time                                                             
           197,230      page-faults                                                           
11,546,530,116,698      L1-dcache-loads                                                         (43.75%)
 1,175,863,490,126      L1-dcache-load-misses            #   10.18% of all L1-dcache accesses   (43.75%)
 1,102,123,802,619      L1-dcache-stores                                                        (43.75%)
       523,788,201      L1-icache-load-misses                                                   (43.75%)
   957,922,755,311      LLC-loads                                                               (25.00%)
     2,861,865,612      LLC-load-misses                  #    0.30% of all L1-icache accesses   (25.00%)
        44,348,742      LLC-stores                                                              (12.50%)
        18,226,105      LLC-store-misses                                                        (12.50%)
11,552,726,081,292      dTLB-loads                                                              (18.75%)
   518,437,302,979      dTLB-load-misses                 #    4.49% of all dTLB cache accesses  (25.00%)
 1,102,405,097,230      dTLB-stores                                                             (25.00%)
        20,618,786      dTLB-store-misses                                                       (25.00%)
        13,713,203      iTLB-load-misses                                                        (25.00%)
        85,614,929      branch-misses                    #    0.02% of all branches             (31.25%)
   553,363,279,103      branch-instructions                                                     (37.50%)
   104,104,517,082      bus-cycles                                                              (43.75%)

    4426.857946943 seconds time elapsed

    4425.380384000 seconds user
       0.547962000 seconds sys

*/