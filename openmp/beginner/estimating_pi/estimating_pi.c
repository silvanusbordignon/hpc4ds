#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) { 

    // Get number of threads in the thread pool
    int thread_count = strtol(argv[2], NULL, 10);

    // Get how many iterations of the loop to run
    long n = strtol(argv[1], NULL, 10);

    double factor;
    double sum;
    double pi_approx;
    
    // Starting a timer
    double begin = omp_get_wtime();

    sum = 0.0;
    # pragma omp parallel for num_threads(thread_count) reduction(+:sum) private(factor)
    for (long k = 0; k < n; k++) {
        if (k % 2 == 0) {
            factor = 1.0;
        }
        else {
            factor = -1.0;
        }
        sum += factor / (2 * k + 1);
    }
    pi_approx = 4.0 * sum;

    // Ending a timer and computing how much time has passed
    double end = omp_get_wtime();
    double time_spent = (double)(end - begin);

    printf("PI estimation with %ld iterations: %.15lf\n", n, pi_approx);
    printf("Time taken: %lfs", time_spent);

    return 0;
}
