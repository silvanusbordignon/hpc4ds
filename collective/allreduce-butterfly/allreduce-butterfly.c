/*
    Implement a custom Allreduce function that works the same as the one
    provided by MPI, but works by creating a butterfly network to handle
    sharing the data and computing the sums
*/

#include <mpi.h>
#include <stdio.h>

int main() {

    // Initializing the MPI architecture

    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Generating the values we will sum together

    // Calling our custom Allreduce function

    // Tearing down the MPI structures

    MPI_Finalize();

    return 0;
}