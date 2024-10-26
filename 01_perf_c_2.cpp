/*
Author: akashmaji(@iisc.ac.in)
File  : 01_perf_c_2.cpp
*/

#include<stdio.h>
#include<stdlib.h>

#define ROWS 2048
#define COLS 2048
#define SIZE 2048

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
gcc 01_perf_c_2.cpp -o 01_perf_c_2

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_2

*/

/*------------SAMPLE-------------*/
/*
 
akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_c_2
Success: memory allocated!
OK

 Performance counter stats for './01_perf_c_2':

    28,397,708,706 ns   duration_time                                                         
    28,364,288,000 ns   user_time                                                             
        32,999,000 ns   system_time                                                           
     1,157,427,009      cache-references                                                        (41.66%)
       231,184,929      cache-misses                     #   19.97% of all cache refs           (41.67%)
       815,400,182      L1-dcache-load-misses                                                   (41.67%)
           534,336      L1-icache-load-misses                                                   (41.67%)
           691,956      dTLB-load-misses                                                        (41.68%)
       289,198,513      l1_dtlb_misses                                                          (41.67%)
           677,366      l2_dtlb_misses                                                          (41.67%)
   441,110,427,734      instructions                     #    3.99  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
   110,476,350,465      cpu-cycles                                                              (41.66%)
     1,113,083,779      stalled-cycles-frontend          #    1.01% frontend cycles idle        (41.66%)
     9,196,297,776      branch-instructions                                                     (41.66%)
       174,549,491      branch-misses                    #    1.90% of all branches             (41.66%)
            12,381      page-faults                                                           

      28.397708706 seconds time elapsed

      28.364288000 seconds user
       0.032999000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/

/*
Run Perf: perf stat -e duration_time,user_time,page-faults,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-load-misses,branch-misses,branch-instructions,bus-cycles ./01_perf_c_2
*/

/*
Performance counter stats for './01_perf_c_2':

    24,635,800,144 ns   duration_time                                                         
    24,623,036,000 ns   user_time                                                             
            12,376      page-faults                                                           
   190,260,678,597      L1-dcache-loads                                                         (43.74%)
        65,210,685      L1-dcache-load-misses            #    0.03% of all L1-dcache accesses   (43.74%)
    17,594,147,665      L1-dcache-stores                                                        (43.74%)
         3,586,939      L1-icache-load-misses                                                   (43.74%)
         1,576,002      LLC-loads                                                               (25.01%)
           525,688      LLC-load-misses                  #   33.36% of all L1-icache accesses   (25.01%)
           597,245      LLC-stores                                                              (12.50%)
           408,272      LLC-store-misses                                                        (12.50%)
   189,246,743,067      dTLB-loads                                                              (18.75%)
           187,679      dTLB-load-misses                 #    0.00% of all dTLB cache accesses  (25.00%)
    17,525,520,826      dTLB-stores                                                             (25.00%)
            91,484      dTLB-store-misses                                                       (25.01%)
             3,867      iTLB-load-misses                                                        (25.01%)
       136,601,847      branch-misses                    #    1.49% of all branches             (31.26%)
     9,169,744,340      branch-instructions                                                     (37.51%)
       583,186,121      bus-cycles                                                              (43.75%)

      24.635800144 seconds time elapsed

      24.623036000 seconds user
       0.011999000 seconds sys



*/