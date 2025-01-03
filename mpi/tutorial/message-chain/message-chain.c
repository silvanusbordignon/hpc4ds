#include <stdio.h>
#include <mpi.h>

int main(void) {

	MPI_Init(NULL, NULL);

    	int comm_sz;
    	int my_rank;

    	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	if (comm_sz == 1) {
		
		MPI_Finalize();
		return 0;
	}

	int counter = 0;

	printf("[%d] UP\n", my_rank);

	if (my_rank == 0) {
		MPI_Send(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	}
	else if (my_rank == comm_sz - 1) {
		MPI_Recv(&counter, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		counter += 1;
	}
	else {
		MPI_Recv(&counter, 1, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		counter += 1;
		MPI_Send(&counter, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
	}	

	printf("[%d] END WITH %d\n", my_rank, counter);

	MPI_Finalize();
	return 0;
}
