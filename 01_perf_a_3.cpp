/*
Author: akashmaji(@iisc.ac.in)
File:   01_perf_a_3.cpp
*/

#include<stdio.h>
#include<stdlib.h>

#define ROWS 2048
#define COLS 2048
#define SIZE 2048

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
    // loop order is (k, i, j)
    for(int k = 0; k < SIZE; k++){
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLS; j++){
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
gcc 01_perf_a_3.cpp -o 01_perf_a_3

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_3

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ gcc 01_perf_a_3.cpp -o 01_perf_a_3
akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_3

 Performance counter stats for './01_perf_a_3':

    23,669,639,649 ns   duration_time                                                         
    23,626,690,000 ns   user_time                                                             
        34,002,000 ns   system_time                                                           
     1,489,746,524      cache-references                                                        (41.67%)
       171,889,600      cache-misses                     #   11.54% of all cache refs           (41.66%)
       622,816,586      L1-dcache-load-misses                                                   (41.67%)
           232,545      L1-icache-load-misses                                                   (41.67%)
        17,220,821      dTLB-load-misses                                                        (41.67%)
        18,717,570      l1_dtlb_misses                                                          (41.67%)
        17,226,782      l2_dtlb_misses                                                          (41.67%)
   422,086,239,907      instructions                     #    4.28  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
    98,552,702,108      cpu-cycles                                                              (41.67%)
       367,482,053      stalled-cycles-frontend          #    0.37% frontend cycles idle        (41.66%)
     8,799,040,215      branch-instructions                                                     (41.66%)
        13,055,985      branch-misses                    #    0.15% of all branches             (41.66%)
            12,379      page-faults                                                           

      23.669639649 seconds time elapsed

      23.626690000 seconds user
       0.034002000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
/*-------------------------*/

/*
Run perf:perf stat -e duration_time,user_time,page-faults,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-load-misses,branch-misses,branch-instructions,bus-cycles ./01_perf_a_3


*/
/*

Performance counter stats for './01_perf_a_3':

    24,600,546,672 ns   duration_time                                                         
    24,579,683,000 ns   user_time                                                             
            12,377      page-faults                                                           
   180,493,924,548      L1-dcache-loads                                                         (43.72%)
       566,698,299      L1-dcache-load-misses            #    0.31% of all L1-dcache accesses   (43.73%)
    17,280,379,677      L1-dcache-stores                                                        (43.75%)
         4,980,158      L1-icache-load-misses                                                   (43.77%)
        10,044,197      LLC-loads                                                               (25.04%)
         6,827,789      LLC-load-misses                  #   67.98% of all L1-icache accesses   (25.04%)
           579,777      LLC-stores                                                              (12.51%)
           321,250      LLC-store-misses                                                        (12.49%)
   180,800,981,910      dTLB-loads                                                              (18.74%)
         6,763,029      dTLB-load-misses                 #    0.00% of all dTLB cache accesses  (24.98%)
    17,344,176,075      dTLB-stores                                                             (24.98%)
           296,218      dTLB-store-misses                                                       (24.98%)
            45,773      iTLB-load-misses                                                        (24.98%)
         4,621,447      branch-misses                    #    0.05% of all branches             (31.22%)
     8,774,585,811      branch-instructions                                                     (37.46%)
       575,552,191      bus-cycles                                                              (43.71%)

      24.600546672 seconds time elapsed

      24.579683000 seconds user
       0.019999000 seconds sys

*/