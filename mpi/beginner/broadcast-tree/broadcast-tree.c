/*
    Implement a “broadcast” by using send and receive (tree)
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

    int value;

    if (my_rank == 0) {
        
        srand(time(NULL));
        value = rand() % 100;

        int left_rank = my_rank * 2 + 1;
        int right_rank = my_rank * 2 + 2;

        if (left_rank < comm_sz) {
            MPI_Send(&value, 1, MPI_INT, left_rank, 0, MPI_COMM_WORLD);

            if (right_rank < comm_sz) {
                MPI_Send(&value, 1, MPI_INT, right_rank, 0, MPI_COMM_WORLD);
            }
        }
    }
    else {
        // All but the first process have to receive something

        int parent_rank = (my_rank + my_rank % 2) / 2 - 1;
        MPI_Recv(&value, 1, MPI_INT, parent_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[%d] Received %d from %d\n", my_rank, value, parent_rank);

        int left_rank = my_rank * 2 + 1;
        int right_rank = my_rank * 2 + 2;

        if (left_rank < comm_sz) {
            MPI_Send(&value, 1, MPI_INT, left_rank, 0, MPI_COMM_WORLD);

            if (right_rank < comm_sz) {
                MPI_Send(&value, 1, MPI_INT, right_rank, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();

    return 0;
}