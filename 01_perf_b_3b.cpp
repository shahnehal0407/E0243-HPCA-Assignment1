/*
Author : akashmaji(@iisc.ac.in)
File   : 01_perf_b_3b.cpp
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
    // loop order is (k, i, j)
    for(int k = 0; k < SIZE; k++){
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLS; j++){
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
gcc 01_perf_b_3b.cpp -o 01_perf_b_3b

Run perf:
perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_3b

*/

/*------------SAMPLE-------------*/
/*

akash@victus:~/Desktop/HPCA$ perf stat -e duration_time,user_time,system_time,cache-references,cache-misses,L1-dcache-load-misses,L1-icache-load-misses,dTLB-load-misses,l1_dtlb_misses,l2_dtlb_misses,instructions,cpu-cycles,stalled-cycles-frontend,branch-instructions,branch-misses,page-faults ./01_perf_b_3b

 Performance counter stats for './01_perf_b_3b':

    21,185,127,332 ns   duration_time                                                         
    21,158,888,000 ns   user_time                                                             
        13,001,000 ns   system_time                                                           
     1,257,349,662      cache-references                                                        (41.67%)
        34,977,393      cache-misses                     #    2.78% of all cache refs           (41.68%)
       666,979,566      L1-dcache-load-misses                                                   (41.68%)
           159,643      L1-icache-load-misses                                                   (41.67%)
            30,440      dTLB-load-misses                                                        (41.66%)
           254,676      l1_dtlb_misses                                                          (41.66%)
            29,205      l2_dtlb_misses                                                          (41.66%)
   387,572,600,612      instructions                     #    4.67  insn per cycle            
                                                  #    0.00  stalled cycles per insn     (41.66%)
    82,968,968,487      cpu-cycles                                                              (41.66%)
       283,510,453      stalled-cycles-frontend          #    0.34% frontend cycles idle        (41.66%)
     8,764,194,037      branch-instructions                                                     (41.66%)
         9,677,475      branch-misses                    #    0.11% of all branches             (41.67%)
                78      page-faults                                                           

      21.185127332 seconds time elapsed

      21.158888000 seconds user
       0.013001000 seconds sys


akash@victus:~/Desktop/HPCA$ 

*/
