#include <stdio.h>
#include <mpi.h>

int main(void) {

    MPI_Init(NULL, NULL);

    int comm_sz;
    int my_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    if (my_rank == 0) {
        printf("Hello, World! I'm the first process, ID %d\n", my_rank);
    }
    else if (my_rank == comm_sz - 1) {
        printf("Hello, World! I'm the last process, ID %d\n", my_rank);
    }
    else {
        if (my_rank % 2 == 0) {
            printf("Hello, World! I'm an even process, ID %d\n", my_rank);
        }
        else {
            printf("Hello, World! I'm an odd process, ID %d\n", my_rank);
        }
    }

    MPI_Finalize();

    return 0;
}

