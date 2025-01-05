#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// This function should always take the same time to execute
void equal_function() {

    int z;

    for (int i = 0; i < 1000; i++) {

        int x = 10;
        int y = 20;

        z = x + y;
        z = z + z;
    }
}

// This function takes a random amount of time to run
void random_function() {
    for (long i = 0; i < rand() * 10000; i++);
}        

int main(int argc, char **argv) { 

    srand(time(NULL));

    // Get the number of iterations to run and the thread count
    long n = strtol(argv[1], NULL, 10);
    int thread_count = strtol(argv[2], NULL, 10);
    
    double begin, end;

    printf("--------------------------------\n");
    printf("Thread count: %d\n", thread_count);
    printf("Iterations per for loop: %ld\n", n);
    printf("--------------------------------\n\n");

    // Serial

    begin = omp_get_wtime();
    for (long i = 0; i < n; i++) equal_function();
    end = omp_get_wtime();
    printf("equal, serial: %lf\n", (double)(end - begin));

    begin = omp_get_wtime();
    for (long i = 0; i < n; i++) random_function();
    end = omp_get_wtime();
    printf("random, serial: %lf\n\n", (double)(end - begin));

    // Parallel, static scheduling

    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(static)
    for (long i = 0; i < n; i++) equal_function();
    end = omp_get_wtime();
    printf("equal, parallel static: %lf\n", (double)(end - begin));
    
    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(static)
    for (long i = 0; i < n; i++) random_function();
    end = omp_get_wtime();
    printf("random, parallel static: %lf\n\n", (double)(end - begin));
    
    // Parallel, dynamic scheduling

    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(dynamic)
    for (long i = 0; i < n; i++) equal_function();
    end = omp_get_wtime();
    printf("equal, parallel dynamic: %lf\n", (double)(end - begin));
    
    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(dynamic)
    for (long i = 0; i < n; i++) random_function();
    end = omp_get_wtime();
    printf("random, parallel dynamic: %lf\n\n", (double)(end - begin));                    
    
    // Parallel, guided scheduling

    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(guided)
    for (long i = 0; i < n; i++) equal_function();
    end = omp_get_wtime();
    printf("equal, parallel guided: %lf\n", (double)(end - begin));
    
    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(guided)
    for (long i = 0; i < n; i++) random_function();
    end = omp_get_wtime();
    printf("random, parallel guided: %lf\n\n", (double)(end - begin));
    
    // Parallel, automatic scheduling

    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(auto)
    for (long i = 0; i < n; i++) equal_function();
    end = omp_get_wtime();
    printf("equal, parallel auto: %lf\n", (double)(end - begin));
    
    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(auto)
    for (long i = 0; i < n; i++) random_function();
    end = omp_get_wtime();
    printf("random, parallel auto: %lf\n\n", (double)(end - begin));
    
    // Parallel, runtime scheduling

    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(runtime)
    for (long i = 0; i < n; i++) equal_function();
    end = omp_get_wtime();
    printf("equal, parallel runtime: %lf\n", (double)(end - begin));
    
    begin = omp_get_wtime();
    #pragma omp parallel for num_threads(thread_count) schedule(runtime)
    for (long i = 0; i < n; i++) random_function();
    end = omp_get_wtime();
    printf("random, parallel runtime: %lf\n\n", (double)(end - begin));

    return 0;
}
