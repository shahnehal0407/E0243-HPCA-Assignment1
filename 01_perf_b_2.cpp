/*
Author : akashmaji(@iisc.ac.in)
File   : 01_perf_b_2.cpp
*/
#include<stdio.h>
#include<stdlib.h>
#include<linux/mman.h>
#include<sys/mman.h>

#define ROWS 2048
#define COLS 2048
#define SIZE 2048

#define LIMIT 1000

#define MB_2 (2*1024*1024)

/*
Some Calculation on large pages (2MB)
matrix size : 2048 * 2048 elements
            : 2^22 ints
            : 2^24 B
            : 16 MB
            : 8 huge pages per matrix
total size  : 3 matrices A, B, C
            : 3 * 8 huge pages each
            : 24 huge pages
That means, we must have 24 huge pages each of size 2MB allocated
*/

#define NMAP_ALLOC_SIZE (MB_2 * 8)

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
gcc 01_perf_b_2.cpp -o 01_perf_b_2

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_2

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_2

 Performance counter stats for './01_perf_b_2':

    32,056,312,401 ns   duration_time                                                         
    32,050,390,000 ns   user_time                                                             
         4,999,000 ns   system_time                                                           
    20,666,811,870      cache-references                                                        (41.66%)
     8,780,382,137      cache-misses                     #   42.49% of all cache refs           (41.66%)
     9,505,506,242      L1-dcache-load-misses                                                   (41.67%)
         1,502,040      L1-icache-load-misses                                                   (41.67%)
            11,004      dTLB-load-misses                                                        (41.67%)
           227,808      l1_dtlb_misses                                                          (41.67%)
             9,459      l2_dtlb_misses                                                          (41.67%)
   385,878,095,543      instructions                     #    2.85  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
   135,624,603,694      cpu-cycles                                                              (41.66%)
     1,867,981,693      stalled-cycles-frontend          #    1.38% frontend cycles idle        (41.66%)
     8,782,285,992      branch-instructions                                                     (41.66%)
        12,358,126      branch-misses                    #    0.14% of all branches             (41.66%)
                78      page-faults                                                           

      32.056312401 seconds time elapsed

      32.050390000 seconds user
       0.004999000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
/*
Run Perf :perf stat -e duration_time,user_time,page-faults,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-load-misses,branch-misses,branch-instructions,bus-cycles ./01_perf_b_2


*/
/*

Performance counter stats for './01_perf_b_2':

    92,630,806,364 ns   duration_time                                                         
    92,627,513,000 ns   user_time                                                             
                76      page-faults                                                           
   146,274,782,105      L1-dcache-loads                                                         (43.74%)
     9,166,601,291      L1-dcache-load-misses            #    6.27% of all L1-dcache accesses   (43.75%)
    17,308,685,726      L1-dcache-stores                                                        (43.75%)
         9,859,822      L1-icache-load-misses                                                   (43.76%)
     2,913,869,006      LLC-loads                                                               (25.01%)
       954,685,548      LLC-load-misses                  #   32.76% of all L1-icache accesses   (25.01%)
         6,287,668      LLC-stores                                                              (12.50%)
         2,707,380      LLC-store-misses                                                        (12.50%)
   146,317,418,345      dTLB-loads                                                              (18.75%)
            12,430      dTLB-load-misses                 #    0.00% of all dTLB cache accesses  (25.00%)
    17,301,784,980      dTLB-stores                                                             (25.00%)
            93,163      dTLB-store-misses                                                       (24.99%)
            15,404      iTLB-load-misses                                                        (24.99%)
         4,683,516      branch-misses                    #    0.05% of all branches             (31.24%)
     8,767,129,620      branch-instructions                                                     (37.49%)
     2,211,446,148      bus-cycles                                                              (43.74%)

      92.630806364 seconds time elapsed

      92.627513000 seconds user
       0.000000000 seconds sys



*/