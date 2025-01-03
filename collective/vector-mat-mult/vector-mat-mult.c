/*
    Perform matrix-vector multiplication

    This program will:
    - take as command line arguments the sizes of a matrix and vector
    - inizialize a matrix and vector of given dimensions with random doubles
    - time how long it takes to serially multiply the vector by the matrix
    - time how long it takes to multiply in parallel the vector by the matrix
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>

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

// Fill a vector with random values
void rand_initialize_vector(double *x, int start, int len) {

    for (int i = start; i < start + len; i++) {
        x[i] = ((double) rand() / RAND_MAX);
    }
}

// Fill a matrix with random values
void rand_initialize_matrix(double *a, int m, int n) {

    for (int i = 0; i < m; i++) {
        rand_initialize_vector(a, i * n, n);
    }
}

void print_double_vect(double *x, int start, int len) {

    for (int i = start; i < start + len; i++) {
        printf("%lf ", x[i]);
    }
    printf("\n");
}

void print_int_vect(int *x, int start, int len) {

    for (int i = start; i < start + len; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

void print_double_matrix(double *a, int m, int n) {
    
    for (int i = 0; i < m; i++) {
        print_double_vect(a, i * n, n);
    }
}

// SERIALLY multiply a matrix by a vector, storing the result in a given vector
void serial_mat_vect_mult(double *a, double *x, double *y, int m, int n) {
    
    for (int i = 0; i < m; i++) {
        y[i] = 0;
        for (int j = 0; j < n; j++) {
            y[i] += a[i * n + j] * x[j];
        }
    }
}

// Divide total by how_many_parts, then return the count at position id
void divide_in_parts(int total, int how_many_parts, int *counts) {

    int i = 0;
    for (i = 0; i < how_many_parts; i++) {
        counts[i] = 0;
    }

    i = 0;
    while (total > 0) {
        counts[i] += 1;
        i = (i + 1) % how_many_parts;
        total--;
    }
}

// PARALLEL multiplication of a matrix by a vector, storing the result
// The function takes in single rows of a and y, while x is taken in full
void parallel_mat_vect_mult(double *local_a, double *x, double *local_y, int local_m, int n, MPI_Comm comm) {

    // Each process computes their portion of the result
    for (int local_i = 0; local_i < local_m; local_i++) {
        local_y[local_i] = 0.0;
        for (int j = 0; j < n; j++) {
            local_y[local_i] += local_a[local_i * n + j] * x[j];
        }
    }
}

// Return 0 if two arrays have the same elements, otherwise return 1
int compare_arrays(double *a, double *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i] != b[i]) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {

    // Rudimental input valudation: the given dimensions must be positive

    if (argc != 3) {
        
        printf("usage: %s [m] [n]\n", argv[0]);
        return 0;
    }

    if (strtol(argv[1], NULL, 10) < 1 || strtol(argv[2], NULL, 10) < 1) {
        
        printf("command line arguments must be positive\n");
        return 0;
    }

    int m = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);

    // Initialize the MPI infrasturcture and retrieve the communicator's info

    int comm_sz;
    int my_rank; 

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Declarations that need to be common
    
    srand(time(NULL));

    double *a, *x, *y;

    clock_t start, end;
    double cpu_time_used;

    // Allocate the space for the input vector
    x = safe_malloc(n * sizeof(double));

    // The main process runs the serial operations
    if (my_rank == 0) {

        printf("Setup\n");

        // Print memory needed to store the the matrix and the input/output vectors
        printf("\tspace needed: %ldB\n", sizeof(double) * (m * n + 3 * n));

        // Allocate the space for the input structures

        a = safe_malloc(m * n * sizeof(double));
        y = safe_malloc(m * sizeof(double));

        // Filling the data structures with random data, timed

        start = clock();
        
        rand_initialize_matrix(a, m, n);
        rand_initialize_vector(x, 0, n);

        end = clock();

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\ttime to initialize: %lfs\n", cpu_time_used);

        // printf("\nMatrix:\n");
        // print_double_matrix(a, m, n);
        // printf("\nVector:\n");
        // print_double_vect(x, 0, n);

        // SERIAL matrix-vector multiplication, timed

        start = clock();
        serial_mat_vect_mult(a, x, y, m, n);
        end = clock();

        printf("\nSerial multiplication\n");

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\ttime: %lfs\n", cpu_time_used);
        // printf("\tresult: ");
        // print_double_vect(y, 0, m);
    }

    // -- TIMED - start of collective operations

    if (my_rank == 0) {
        start = clock();
    }

    // Compute the row intervals that each process will receive and compute

    double *local_a;
    double *local_y;
    int local_m;

    // Additional array in which to save the result of the parallel operation
    double *z;

    int *m_counts = safe_malloc(comm_sz * sizeof(int));
    int *items_counts = safe_malloc(comm_sz * sizeof(int));
    divide_in_parts(m, comm_sz, m_counts);

    local_m = m_counts[my_rank];

    int *items_displacements = safe_malloc(comm_sz * sizeof(int));
    int *m_displacements = safe_malloc(comm_sz * sizeof(int));
    for (int i = 0; i < comm_sz; i++) {
        items_displacements[i] = 0;
        items_counts[i] = m_counts[i] * n;
    }

    int items_sum = 0;
    int m_sum = 0;

    for (int i = 0; i < comm_sz; i++) {
     
        items_displacements[i] = items_sum;
        m_displacements[i] = m_sum;

        m_sum += m_counts[i];
        items_sum += items_counts[i];
    }

    // Once the number of rows is known for each vector, time to allocate
    
    local_a = safe_malloc(local_m * n * sizeof(double));
    local_y = safe_malloc(local_m * sizeof(double));
    
    if (my_rank == 0) {
        z = safe_malloc(m * sizeof(double));
    }
    
    // Scatter around the matrix created by process 0
    MPI_Scatterv(a, items_counts, items_displacements, MPI_DOUBLE, local_a, items_counts[my_rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // Since the input vector is used in full, everyone needs to have it
    MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // Run the parallel multiplication
    parallel_mat_vect_mult(local_a, x, local_y, local_m, n, MPI_COMM_WORLD);
    // Have the main process rebuild the entire vector
    MPI_Gatherv(local_y, local_m, MPI_DOUBLE, z, m_counts, m_displacements, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // -- TIMED - end of collective operations
    if (my_rank == 0) {
        end = clock();

        printf("\nParallel multiplication:\n");

        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\ttime: %lfs\n", cpu_time_used);

        // printf("\tresult: ");
        // print_double_vect(z, 0, m);

        // Compare the serial and parallel results

        printf("\nSame result computed: %s\n", (compare_arrays(y, z, m) == 0 ? "yes" : "no"));

        // The main process 0 frees the allocated memory
        free(a);
        free(y);
        free(z);
    }

    // Freeing the remaining vectors
    
    free(x);

    free(m_counts);
    free(items_counts);
    free(items_displacements);
    free(m_displacements);
    
    free(local_a);
    free(local_y);

    // Tear down the MPI infrastucture and successfully terminate the program

    MPI_Finalize();
    return 0;
}