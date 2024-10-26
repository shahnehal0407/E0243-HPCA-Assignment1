/*
Author : akashmaji(@iisc.ac.in)
File   : 01_perf_b_1b.cpp
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
    // loop order is (i, j, k)
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
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
gcc 01_perf_b_1b.cpp -o 01_perf_b_1b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_1b

*/

/*------------SAMPLE-------------*/
/*


*/
/*

perf stat -e duration_time,user_time,page-faults,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores,L1-icache-load-misses,LLC-loads,LLC-load-misses,LLC-stores,LLC-store-misses,dTLB-loads,dTLB-load-misses,dTLB-stores,dTLB-store-misses,iTLB-load-misses,branch-misses,branch-instructions,bus-cycles ./01_perf_b_1b

*/
/*
Performance counter stats for './01_perf_b_1b':

11,139,637,268,956 ns   duration_time                                                         
11,136,436,269,000 ns   user_time                                                             
               438      page-faults                                                           
 9,351,843,396,129      L1-dcache-loads                                                         (43.75%)
   554,643,660,292      L1-dcache-load-misses            #    5.93% of all L1-dcache accesses   (43.75%)
 1,103,796,102,446      L1-dcache-stores                                                        (43.75%)
     1,254,965,004      L1-icache-load-misses                                                   (43.75%)
   550,138,866,918      LLC-loads                                                               (25.00%)
   548,435,729,154      LLC-load-misses                  #   99.69% of all L1-icache accesses   (25.00%)
       582,761,944      LLC-stores                                                              (12.50%)
       522,126,714      LLC-store-misses                                                        (12.50%)
 9,353,519,539,316      dTLB-loads                                                              (18.75%)
        13,429,387      dTLB-load-misses                 #    0.00% of all dTLB cache accesses  (25.00%)
 1,103,765,705,725      dTLB-stores                                                             (25.00%)
        11,835,387      dTLB-store-misses                                                       (25.00%)
         1,740,466      iTLB-load-misses                                                        (25.00%)
        99,854,918      branch-misses                    #    0.02% of all branches             (31.25%)
   554,780,482,096      branch-instructions                                                     (37.50%)
   265,443,790,303      bus-cycles                                                              (43.75%)

   11139.637268956 seconds time elapsed

   11136.436269000 seconds user
       0.699963000 seconds sys



*/
