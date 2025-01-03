// Lecture 8 - Exercise: ring

#include <stdio.h>
#include <mpi.h>

int main() {

	MPI_Init(NULL, NULL);

	int comm_sz;
	int my_rank;

	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	int hops = 0;
	
	printf("[%d] ready\n", my_rank);

	if (comm_sz == 1) {
		printf("[%d] only one process\n", my_rank);
	}
	else {
		if (my_rank == 0) {
			MPI_Send(&hops, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
			
			MPI_Recv(&hops, 1, MPI_INT, comm_sz - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			hops += 1;
		}
		else if (my_rank == comm_sz - 1) {
			MPI_Recv(&hops, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			hops += 1;

			MPI_Send(&hops, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
		else {
			MPI_Recv(&hops, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			hops += 1;
			
			MPI_Send(&hops, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
		}
	}

	printf("[%d] hops to get to me: %d\n", my_rank, hops);

	MPI_Finalize();

	return 0;
}
