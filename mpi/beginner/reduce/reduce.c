/*
    Implement a “reduce” by using send and receive + sum over the set of data
    collected. The first process gets data from all the other ones
*/

#include <mpi.h>
#include <stdio.h>

#include <time.h>
#include <stdlib.h>

int main() {

	int comm_sz;
	int my_rank;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int sum = 0;
    int tmp;

    srand(time(NULL));

    if (my_rank == 0) {

        int i = 1;

        while (i < comm_sz) {
            MPI_Recv(&tmp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += tmp;

            printf("[%d] Received %d from %d\n", my_rank, tmp, i);

            i++;
        }

        printf("[%d] Final sum: %d\n", my_rank, sum);
    }
    else {

        // It's not that random, but ok
        // It's a lack of C skills rather than HPC skills
        tmp = rand() % 100;
        MPI_Send(&tmp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        printf("[%d] Sent %d to 0\n", my_rank, tmp);
    }

    MPI_Finalize();

    return 0;
}