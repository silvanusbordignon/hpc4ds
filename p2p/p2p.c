/*
    Take an input value and compute the closest power of 2 that's lesser or equal;
    we'll call this result n and it will be the value we'll use throughout this code

    Take another input value, we'll call it m, and it will be the number of times
    we run a benchmark for each array size we want to test (n)

    ---

    results[0] = statistics for data transfer related to double arrays of size 2^0
    results[1] = statistics for data transfer related to double arrays of size 2^1
            .
            .
    results[n - 1] = statistics for data transfer related to double arrays of size 2^(n - 1)

    ---

    Each result[i] is a matrix containing a row for each time we run a test, so m rows, and
    two columns consisting of the time and the bandwidth registered for that run:
    
    results[i][0] = [[time_0, bandwidth_0], [time_1, bandwidth_1], ..., [time_n, bandwidth_m]]

    ---

    TODO:
    - update the input validation by using strtol and checking for each error case
*/

#include <mpi.h>

#include <math.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    int my_rank;
    int comm_sz;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Input validation

    if (argc != 3) {
        if (my_rank == 0) {
            fprintf(stderr, "Usage: p2p <highest vector size to test> <number of cycles>\n");
        }

        MPI_Finalize();
        return 1;
    }

    long n = atol(argv[1]);
    int m = atoi(argv[2]);

    if (n < 1 || m < 1) {
        if (my_rank == 0) {
            fprintf(stderr, "Invalid arguments: both need to be positive numbers\n");
        }

        MPI_Finalize();
        return 1;
    }

    // Convert n to the closest power of 2 that's lesser or equal to the inputted value
    
    n = pow(2, floor(log2(n)));

    printf("%ld %d\n", n, m);

    MPI_Finalize();

    return 0;
}