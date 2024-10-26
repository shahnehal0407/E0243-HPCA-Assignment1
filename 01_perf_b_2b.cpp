/*
Author : akashmaji(@iisc.ac.in)
File   : 01_perf_b_2b.cpp
*/
#include<stdio.h>
#include<stdlib.h>
#include<linux/mman.h>
#include<sys/mman.h>

#define ROWS 8192
#define COLS 8192
#define SIZE 8192

#define LIMIT 1000

#define MB_2 (2*1024*1024)

/*
Some Calculation on large pages (2MB)
matrix size : 8192 * 8192 elements
            : 2^26 ints
            : 2^28 B
            : 256 MB
            : 128 huge pages per matrix
total size  : 3 matrices A, B, C
            : 3 * 128 huge pages each
            : 384 huge pages
That means, we must have 384 huge pages each of size 2MB allocated
*/

#define NMAP_ALLOC_SIZE (MB_2 * 128)

void print_matrix(int* matrix, int N, int M){
    for(int row = 0; row < N; row++){
        for(int col = 0; col < M; col++){
            printf("%d ", matrix[row * N + col]);
        }
        printf("\n");
    }
}


int main(){
    
    // printf("Allocating...\n");
    // allocate memory dynamically to matrices A, B and C using nmap with appropriate size and flags
    int *matrixA = (int*)mmap(NULL,
                      NMAP_ALLOC_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB,
                      -1, 0);
    int* matrixB = (int*)mmap(NULL,
                      NMAP_ALLOC_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB,
                      -1, 0);
    int* matrixC = (int*)mmap(NULL,
                      NMAP_ALLOC_SIZE,
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_HUGE_2MB,
                      -1, 0);

    // sanity check that nmap() has indeed allocated huge pages
    if(matrixA == NULL || matrixB == NULL || matrixC == NULL){
        printf("Error: nmap() failed to allocate memory!");
        exit(0);
    }

    // printf("Initializing...\n");
    // initialize matrices A and B with some random values within 'LIMIT' and C with zeros
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            matrixA[row * COLS + col] = rand() % LIMIT;
            matrixB[row * COLS + col] = rand() % LIMIT;
            matrixC[row * COLS + col] = 0;
        }
    }

    // char ch;
    // printf("Success: nmap() allocated. Enter a char to continue:");
    // scanf("%c", &ch);

    // do the matrix multiplication C = A * B
    // loop order is (j, i, k)
    for(int j = 0; j < COLS; j++){
        for(int i = 0; i < ROWS; i++){
            for(int k = 0; k < SIZE; k++){
                matrixC[i * COLS + j] += (matrixA[i * COLS + k] * matrixB[k * COLS + j]);
            }
        }
        // printf("Done one row: %d\n", i);
    }
   
    // see matrix C
    // print_matrix(matrixC, ROWS, COLS);


    // printf("Deallocating...\n");
    // unallocate memeory allocated by nmap() using munmap()
    munmap(matrixA, NMAP_ALLOC_SIZE);
    munmap(matrixB, NMAP_ALLOC_SIZE);
    munmap(matrixC, NMAP_ALLOC_SIZE);


    return 0;
}

/*--------Instructions-------------*/
/*

Compile:
gcc 01_perf_b_2b.cpp -o 01_perf_b_2b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_2b

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_2b

 Performance counter stats for './01_perf_b_2b':

    35,615,894,310 ns   duration_time                                                         
    35,497,138,000 ns   user_time                                                             
        47,990,000 ns   system_time                                                           
    20,257,623,068      cache-references                                                        (41.65%)
     8,717,594,489      cache-misses                     #   43.03% of all cache refs           (41.65%)
     9,487,302,940      L1-dcache-load-misses                                                   (41.67%)
         1,476,813      L1-icache-load-misses                                                   (41.67%)
           147,425      dTLB-load-misses                                                        (41.68%)
         2,035,108      l1_dtlb_misses                                                          (41.68%)
           142,900      l2_dtlb_misses                                                          (41.69%)
   387,385,349,621      instructions                     #    2.73  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.68%)
   141,645,284,003      cpu-cycles                                                              (41.67%)
     1,903,872,592      stalled-cycles-frontend          #    1.34% frontend cycles idle        (41.66%)
     8,792,512,269      branch-instructions                                                     (41.65%)
        14,477,488      branch-misses                    #    0.16% of all branches             (41.65%)
                77      page-faults                                                           

      35.615894310 seconds time elapsed

      35.497138000 seconds user
       0.047990000 seconds sys


akash@victus:~/Desktop/HPCA$ 
*/

/*


Run Perf: perf stat -e duration_time,user_time,page-faults,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-load-misses,branch-misses,branch-instructions,bus-cycles ./01_perf_b_2b


*/


/*
 Performance counter stats for './01_perf_b_2b':

11,134,738,841,697 ns   duration_time                                                         
11,131,221,227,000 ns   user_time                                                             
               436      page-faults                                                           
 9,351,893,672,538      L1-dcache-loads                                                         (43.75%)
   585,449,138,806      L1-dcache-load-misses            #    6.26% of all L1-dcache accesses   (43.75%)
 1,103,650,129,000      L1-dcache-stores                                                        (43.75%)
     1,277,390,048      L1-icache-load-misses                                                   (43.75%)
   550,133,261,235      LLC-loads                                                               (25.00%)
   548,151,801,003      LLC-load-misses                  #   99.64% of all L1-icache accesses   (25.00%)
       211,207,525      LLC-stores                                                              (12.50%)
       101,606,528      LLC-store-misses                                                        (12.50%)
 9,353,470,761,736      dTLB-loads                                                              (18.75%)
        18,071,320      dTLB-load-misses                 #    0.00% of all dTLB cache accesses  (25.00%)
 1,103,699,301,055      dTLB-stores                                                             (25.00%)
        12,470,670      dTLB-store-misses                                                       (25.00%)
         2,558,554      iTLB-load-misses                                                        (25.00%)
       102,508,848      branch-misses                    #    0.02% of all branches             (31.25%)
   554,820,377,522      branch-instructions                                                     (37.50%)
   265,344,609,240      bus-cycles                                                              (43.75%)

   11134.738841697 seconds time elapsed

   11131.221227000 seconds user
       1.083942000 seconds sys

*/