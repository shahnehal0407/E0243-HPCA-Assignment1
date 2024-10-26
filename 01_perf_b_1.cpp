/*
Author : akashmaji(@iisc.ac.in)
File   : 01_perf_b_1.cpp
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
gcc 01_perf_b_1.cpp -o 01_perf_b_1

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_1

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_1

 Performance counter stats for './01_perf_b_1':

    34,485,617,884 ns   duration_time                                                         
    34,480,703,000 ns   user_time                                                             
         2,999,000 ns   system_time                                                           
    19,434,673,409      cache-references                                                        (41.66%)
     8,754,939,240      cache-misses                     #   45.05% of all cache refs           (41.66%)
     8,918,352,331      L1-dcache-load-misses                                                   (41.66%)
         1,165,334      L1-icache-load-misses                                                   (41.66%)
             8,862      dTLB-load-misses                                                        (41.67%)
           226,596      l1_dtlb_misses                                                          (41.67%)
             8,919      l2_dtlb_misses                                                          (41.67%)
   386,134,016,510      instructions                     #    2.72  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.67%)
   141,940,134,964      cpu-cycles                                                              (41.67%)
     1,365,236,785      stalled-cycles-frontend          #    0.96% frontend cycles idle        (41.67%)
     8,802,902,950      branch-instructions                                                     (41.67%)
        13,127,205      branch-misses                    #    0.15% of all branches             (41.66%)
                78      page-faults                                                           

      34.485617884 seconds time elapsed

      34.480703000 seconds user
       0.002999000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
