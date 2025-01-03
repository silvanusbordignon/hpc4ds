/*
    Implement a “reduce” by using send and receive + sum (or avg, min, max)
    over the set of data collected, and then broadcast the result
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

    int sum = 0;
    int tmp;

    if (my_rank == 0) {
        
        int i = 1;

        while (i < comm_sz) {
            MPI_Recv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += tmp;

            printf("[%d] Received %d from %d\n", my_rank, tmp, i);

            i++;
        }

        i = 1;

        while (i < comm_sz) {
            MPI_Send(&sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            printf("[%d] Sent %d to %d\n", my_rank, sum, i);

            i++;
        }
    }
    else {
        srand(time(NULL));
        tmp = rand() % 100;

        // This, as well as many other messages, are not synchronized
        // meaning the "sent" prints may come after the sum has already be sent
        // by the first process (so having addends, computing and sending sums,
        // before the "sent to" messages are printed)
        MPI_Send(&tmp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("[%d] Sent %d to 0\n", my_rank, tmp);

        MPI_Recv(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[%d] Received %d from 0\n", my_rank, sum);
    }

    MPI_Finalize();

    return 0;
}
