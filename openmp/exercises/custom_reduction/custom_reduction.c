#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) { 

    // Get command line arguments
    int thread_count = strtol(argv[1], NULL, 10);
    long long iterations = strtol(argv[2], NULL, 10);

    long long sum_1, sum_2, sum_3, sum_4, sum_5;
    double begin, end;

    // Serial for loop: slow, default

    begin = omp_get_wtime();
    sum_1 = 0;
    for (long long i = 0; i < iterations; i++) {
        sum_1 = sum_1 + 3;
    }
    end = omp_get_wtime();

    printf("[serial] for: sum %lld, time %lfs\n", sum_1, (double) (end - begin));

    // Parallel for loop: race conditions, wrong result

    begin = omp_get_wtime();
    sum_2 = 0;
    # pragma omp parallel for num_threads(thread_count)
    for (long long i = 0; i < iterations; i++) {
        sum_2 = sum_2 + 3;
    }
    end = omp_get_wtime();

    printf("[%d threads] parallel for: sum %lld, time %lfs\n", thread_count, sum_2, (double) (end - begin));

    // Parallel for loop with reduction: efficient, parallel, lovely

    begin = omp_get_wtime();
    sum_3 = 0;
    # pragma omp parallel for num_threads(thread_count) reduction(+:sum_3)
    for (long long i = 0; i < iterations; i++) {
        sum_3 = sum_3 + 3;
    }
    end = omp_get_wtime();

    printf("[%d threads] reduction for: sum %lld, time %lfs\n", thread_count, sum_3, (double) (end - begin));

    // Parallel section with atomic: parallel, slowed down by atomic directive

    begin = omp_get_wtime();
    sum_4 = 0;
    # pragma omp parallel for num_threads(thread_count)
    for (long long i = 0; i < iterations; i++) {
        # pragma omp atomic
        sum_4 += 3;
    }
    end = omp_get_wtime();

    printf("[%d threads] parallel atomic: sum %lld, time %lfs\n", thread_count, sum_4, (double) (end - begin));

    // Parallel section with critical: parallel, slowed down by critical directive

    begin = omp_get_wtime();
    sum_5 = 0;
    # pragma omp parallel for num_threads(thread_count)
    for (long long i = 0; i < iterations; i++) {
        # pragma omp critical
        sum_5 = sum_5 + 3;
    }
    end = omp_get_wtime();

    printf("[%d threads] parallel critical: sum %lld, time %lfs\n\n", thread_count, sum_5, (double) (end - begin));

    return 0;
}