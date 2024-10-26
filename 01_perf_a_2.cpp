/*
Author: akashmaji(@iisc.ac.in)
File:   01_perf_a_2.cpp
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
gcc 01_perf_a_2.cpp -o 01_perf_a_2

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_2

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ gcc 01_perf_a_2.cpp -o 01_perf_a_2
akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_a_2

 Performance counter stats for './01_perf_a_2':

    28,041,742,595 ns   duration_time                                                         
    28,018,704,000 ns   user_time                                                             
        19,999,000 ns   system_time                                                           
    27,279,233,650      cache-references                                                        (41.66%)
    10,616,561,619      cache-misses                     #   38.92% of all cache refs           (41.66%)
    10,412,450,261      L1-dcache-load-misses                                                   (41.66%)
           318,155      L1-icache-load-misses                                                   (41.67%)
     5,781,698,896      dTLB-load-misses                                                        (41.67%)
     8,692,025,772      l1_dtlb_misses                                                          (41.68%)
     5,779,218,928      l2_dtlb_misses                                                          (41.68%)
   421,984,436,006      instructions                     #    3.65  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.68%)
   115,563,927,816      cpu-cycles                                                              (41.67%)
       356,029,252      stalled-cycles-frontend          #    0.31% frontend cycles idle        (41.66%)
     8,799,381,269      branch-instructions                                                     (41.66%)
        13,453,817      branch-misses                    #    0.15% of all branches             (41.66%)
            12,379      page-faults                                                           

      28.041742595 seconds time elapsed

      28.018704000 seconds user
       0.019999000 seconds sys


akash@victus:~/Desktop/HPCA$

*/
/*-------------------------*/
/*
run perf:perf stat -e duration_time,user_time,page-faults,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-load-misses,branch-misses,branch-instructions,bus-cycles ./01_perf_a_2

*/
/*

Performance counter stats for './01_perf_a_2':

    28,747,145,839 ns   duration_time                                                         
    28,713,868,000 ns   user_time                                                             
            12,377      page-faults                                                           
   180,799,591,141      L1-dcache-loads                                                         (43.74%)
    12,931,597,982      L1-dcache-load-misses            #    7.15% of all L1-dcache accesses   (43.76%)
    17,323,845,706      L1-dcache-stores                                                        (43.77%)
         4,192,323      L1-icache-load-misses                                                   (43.77%)
        97,401,129      LLC-loads                                                               (25.02%)
        10,706,600      LLC-load-misses                  #   10.99% of all L1-icache accesses   (25.01%)
           663,832      LLC-stores                                                              (12.49%)
           286,705      LLC-store-misses                                                        (12.49%)
   180,650,953,750      dTLB-loads                                                              (18.74%)
     2,985,825,485      dTLB-load-misses                 #    1.65% of all dTLB cache accesses  (24.99%)
    17,305,298,422      dTLB-stores                                                             (24.99%)
           625,479      dTLB-store-misses                                                       (24.99%)
            63,775      iTLB-load-misses                                                        (24.99%)
         4,557,846      branch-misses                    #    0.05% of all branches             (31.24%)
     8,775,828,417      branch-instructions                                                     (37.49%)
       681,141,936      bus-cycles                                                              (43.74%)

      28.747145839 seconds time elapsed

      28.713868000 seconds user
       0.032002000 seconds sys



*/