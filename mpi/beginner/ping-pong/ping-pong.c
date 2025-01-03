// Lecture 8 - Exercise: ping pong

#include <mpi.h>
#include <stdio.h>

int main() {

	int comm_sz;
	int my_rank;

	MPI_Init(NULL, NULL);

    	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	if (comm_sz != 2) {
		MPI_Finalize();
		return 0;
	}

	char buffer[5];

	if (my_rank == 0) {

		sprintf(buffer, "ping");
		MPI_Send(buffer, sizeof(buffer), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
		printf("[%d] sent a %s\n", my_rank, buffer);
	
		MPI_Recv(buffer, sizeof(buffer), MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("[%d] received a %s\n", my_rank, buffer);
	}
	else {

		MPI_Recv(buffer, sizeof(buffer), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("[%d] received a %s\n", my_rank, buffer);
		
		sprintf(buffer, "pong");
		MPI_Send(buffer, sizeof(buffer), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		printf("[%d] sent a %s\n", my_rank, buffer);
	}

	MPI_Finalize();

	return 0;
}
