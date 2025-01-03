#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void hello();

int main(int argc, char **argv) { 

    // Get number of threads in the thread pool
    int thread_count = strtol(argv[1], NULL, 10);

    #pragma omp parallel num_threads(thread_count)
    hello();

    return 0;
}

void hello() {
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    printf("Hello from thread %d of %d\n", my_rank, thread_count);
}