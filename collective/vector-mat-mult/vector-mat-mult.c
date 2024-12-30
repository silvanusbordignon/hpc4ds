// Perform matrix-vector multiplication

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int main(int argc, char **argv) {

    // Rudimental input valudation: the given dimensions must be positive

    if (argc != 3) {
        printf("usage: %s [m] [n]\n", argv[0]);
        
        MPI_Finalize();
        return 0;
    }

    if (strtol(argv[1], NULL, 10) < 1 || strtol(argv[2], NULL, 10) < 1) {
        printf("command line arguments must be positive\n");
        
        MPI_Finalize();
        return 0;
    }

    // Initialize the MPI infrasturcture and retrieve the communicator's info

    int comm_sz;
    int my_rank; 

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Tear down the MPI infrastucture and successfully terminate the program

    MPI_Finalize();
    return 0;
}
