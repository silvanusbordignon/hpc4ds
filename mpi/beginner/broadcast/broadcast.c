/*
    Implement a “broadcast” by using send and receive (loop-based)
*/

#include <mpi.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

int main() {

    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int random;

    if (my_rank == 0) {
        
        int i = 1;

        while (i < comm_sz) {
        
            srand(time(NULL));
            random = rand() % 100;

            MPI_Send(&random, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            i++;
        }
    }
    else {
        MPI_Recv(&random, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[%d] Received %d from 0\n", my_rank, random);
    }

    MPI_Finalize();

    return 0;
}