#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

void *safe_malloc(size_t n);

void rand_init_vector(double *vector, long long start, long long len);
void rand_init_matrix(double *matrix, long long m, long long n);
int compare_arrays(double *a, double *b, long long n);
void print_double_vect(double *x, long long start, long long len);

void serial_mat_vect_mult(double *a, double *x, double *y, long long m, long long n);
void parallel_1_mat_vect_mult(double *a, double *x, double *y, long long m, long long n, int thread_count);
void parallel_2_mat_vect_mult(double *a, double *x, double *y, long long m, long long n, int thread_count);
void parallel_3_mat_vect_mult(double *a, double *x, double *y, long long m, long long n, int thread_count);

int main(int argc, char **argv) {
    
    if (argc != 4) {
        printf("usage: %s [m] [n] [thread count]\n", argv[0]);
        exit(134);
    }

    for (int i = 1; i <= 3; i++) {
        if (strtol(argv[i], NULL, 10) < 1) {
            printf("dimensions, thread count have to be positive\n");
            exit(134);
        }
    }

    // Matrix and vector dimensions are inputted
    long long m = strtol(argv[1], NULL, 10);
    long long n = strtol(argv[2], NULL, 10);
    
    // Thread count is given as input parameter
    int thread_count = strtol(argv[3], NULL, 10);

    // Declare the needed data structures
    double *a = safe_malloc(m * n * sizeof(double));
    double *x = safe_malloc(n * sizeof(double));
    double *y = safe_malloc(m * sizeof(double));
    double *z = safe_malloc(m * sizeof(double));
    
    // Measure time initialize the matrix and vector

    double begin = omp_get_wtime();
    rand_init_vector(x, 0, n);
    rand_init_matrix(a, m, n);
    double end = omp_get_wtime();
    
    // printf("Space needed: %lldB\n", sizeof(double) * (m * n + 2 * m + n));
    // printf("m: %lld, n: %lld, thread count: %d\n", m, n, thread_count);
    
    printf("Time to initialize: %lf\n", (double)(end - begin));

    // Time serial execution

    begin = omp_get_wtime();
    serial_mat_vect_mult(a, x, y, m, n);
    end = omp_get_wtime();

    printf("Time for serial product: %lf\n\n", (double)(end - begin));

    // Time first parallel version

    begin = omp_get_wtime();
    parallel_1_mat_vect_mult(a, x, z, m, n, thread_count);
    end = omp_get_wtime();

    printf("[%d threads] First parallel version: %lf\n", thread_count, (double)(end - begin));
    printf("[%d threads] Result checks out? %s\n", thread_count,(compare_arrays(y, z, m) == 0) ? "yes" : "no");

    // Time second parallel version

    begin = omp_get_wtime();
    parallel_2_mat_vect_mult(a, x, z, m, n, thread_count);
    end = omp_get_wtime();

    printf("[%d threads] Second parallel version: %lf\n", thread_count, (double)(end - begin));
    printf("[%d threads] Result checks out? %s\n\n", thread_count,(compare_arrays(y, z, m) == 0) ? "yes" : "no");

    // Time third parallel version

    begin = omp_get_wtime();
    parallel_3_mat_vect_mult(a, x, z, m, n, thread_count);
    end = omp_get_wtime();

    printf("[%d threads] Third parallel version: %lf\n", thread_count, (double)(end - begin));
    printf("[%d threads] Result checks out? %s\n\n", thread_count,(compare_arrays(y, z, m) == 0) ? "yes" : "no");


    // Free allocated resources
    free(a);
    free(x);
    free(y);
    free(z);
    
    return 0;
}

// https://stackoverflow.com/a/35027099
void *safe_malloc(size_t n)
{
    void *p = malloc(n);
    if (p == NULL) {
        fprintf(stderr, "Fatal: failed to allocate %zu bytes.\n", n);
        exit(1);
    }
    return p;
}

void rand_init_vector(double *vector, long long start, long long len) {

    for (long long i = start; i < start + len; i++) {
        vector[i] = ((double) rand() / RAND_MAX);
    }
}

void rand_init_matrix(double *matrix, long long m, long long n) {

    for (long long i = 0; i < m; i++) {
        rand_init_vector(matrix, i * n, n);
    }
}

// Return 0 if two arrays have the same elements, otherwise return 1
int compare_arrays(double *a, double *b, long long n) {
    for (long long i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return 1;
        }
    }
    return 0;
}

void print_double_vect(double *x, long long start, long long len) {

    for (long long i = start; i < start + len; i++) {
        printf("%lf ", x[i]);
    }
    printf("\n");
}

void serial_mat_vect_mult(double *a, double *x, double *y, long long m, long long n) {
    
    for (long long i = 0; i < m; i++) {
        y[i] = 0;
        for (long long j = 0; j < n; j++) {
            y[i] += a[i * n + j] * x[j];
        }
    }
}

void parallel_1_mat_vect_mult(double *a, double *x, double *y, long long m, long long n, int thread_count) {
    
    # pragma omp parallel for num_threads(thread_count)
    for (long long i = 0; i < m; i++) {
        y[i] = 0;
        for (long long j = 0; j < n; j++) {
            y[i] += a[i * n + j] * x[j];
        }
    }
}

// problem: sums are being done without any race condition check
void parallel_2_mat_vect_mult(double *a, double *x, double *y, long long m, long long n, int thread_count) {
    
    # pragma omp parallel for num_threads(thread_count)
    for (long long i = 0; i < m; i++) {
        y[i] = 0;
    }

    # pragma omp parallel for num_threads(thread_count) collapse(2)
    for (long long i = 0; i < m; i++) {
        for (long long j = 0; j < n; j++) {
            y[i] += a[i * n + j] * x[j];
        }
    }
}

void parallel_3_mat_vect_mult(double *a, double *x, double *y, long long m, long long n, int thread_count) {
    
    # pragma omp parallel for num_threads(thread_count)
    for (long long i = 0; i < m; i++) {
        y[i] = 0;
    }

    for (long long i = 0; i < m; i++) {
        # pragma omp parallel for num_threads(thread_count) reduction(+: y[i])
        for (long long j = 0; j < n; j++) {
            y[i] += a[i * n + j] * x[j];
        }
    }
}

