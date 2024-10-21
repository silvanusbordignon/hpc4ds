/*
    Write a program to measure the time it takes to send 1, 2, 4, ..., 1M, ...
    bytes (arrays) from one processor to another using MPI_Send and MPI_Recv.

    Basically:
    - take the time
    - send a message
    - receive it back
    - take the time
    - compute the time delta
    - compute the bandwidth
        - from a double array to bytes
        - divide by 2 considering you send AND receive
        - compute bytes/second

    Once this is done, run it using various PBS configurations, but you won't
    need to edit this code during this phase.
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// Maximum buffer size for MPI_Send (INT_MAX)
const long MAX_SIZE = 2147483647;

int main() {

    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // We're benchmarking communications between two processes
    if (comm_sz != 2) {

        if (my_rank == 0) {
            fprintf(stderr, "For this benchmark we require two processes\n");
        }

        MPI_Finalize();
        return 1;
    }

    // This process measures the time, sends and receives data
    if (my_rank == 0) {
        
        // Counting the digits of the max size for formatting purposes

        int digits = 1;
        long temp = MAX_SIZE;

        while (temp / 10 > 0) {
            temp = (long) temp / 10;
            digits += 1;
        }

        printf("%-*s %-*s\n", digits, "n", digits, "time (sec)");

        double t0, t1;

        for (long array_size = 1; array_size <= MAX_SIZE; array_size *= 2) {

            double* array = (double*) malloc(sizeof(double) * array_size);

            t0 = MPI_Wtime();

            MPI_Send(array, array_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(array, array_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            t1 = MPI_Wtime();

            free(array);

            // Computing the MB/s bandwidth
            double bandwidth = (((array_size * 8) / 1000000) / ((t1 - t0) / 2));

            printf("%-*ld %-*f %-*f\n", digits, array_size, digits, t1 - t0, digits, bandwidth);
        }
    }
    // This process receives and sends back data
    else if (my_rank == 1) {

        for (long array_size = 1; array_size <= MAX_SIZE; array_size *= 2) {

            double* array = (double*) malloc(sizeof(double) * array_size);

            MPI_Recv(array, array_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(array, array_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

            free(array);
        }
    }

    MPI_Finalize();

    return 0;
}