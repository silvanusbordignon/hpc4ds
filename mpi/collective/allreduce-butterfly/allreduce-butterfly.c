/*
    Implement a custom Allreduce function that works the same as the one
    provided by MPI, but works by creating a butterfly network to handle
    sharing the data and computing the sums.

    Caveats:
    - only works with a number of processors equal to a power of 2
    - only works with the sum operation
    - temporarily returns 0
*/

#include <mpi.h>
#include <stdio.h>

#include <stdlib.h>
#include <time.h>

#include <math.h>

// Custom implementation of the Allreduce function using only Send and Recv
int MyMPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {

    // To keep using the same function signature I need to repeat these lines

    int my_rank;
    int comm_sz;
    
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // For the actual implementation:

    // Iterators
    int height, iter;

    // Compute source and target nodes
    int group_sz, group_leader_rank, old_group_sz, old_group_leader_rank, source, target, my_half;

    /*
        Assuming multiples of 2, I will have log2(comm_sz) iterations
        
        Each one involves:
        - half the processes in each group sending
        - the rest receiving
        - the second half sends their own value
        - the first half receives those

        This is captured by an innner loop.
    */
    for (height = 0; height < log2(comm_sz); height++) {
    
        // Rudimental computations for source and target nodes

        old_group_sz = (int) pow(2, height);
        old_group_leader_rank = my_rank - my_rank % old_group_sz;
        source = old_group_leader_rank + ((my_rank + (int) pow(2, height - 1)) % old_group_sz);

        group_sz = (int) pow(2, height + 1);
        group_leader_rank = my_rank - my_rank % group_sz;
        target = group_leader_rank + ((my_rank + (int) pow(2, height)) % group_sz);

        my_half = ((my_rank - group_leader_rank) < (group_sz / 2)) ? 0 : 1;

        // The "iter" half is the one sending data
        for (iter = 0; iter < 2; iter++) {

            // If it is my turn, send a value
            if (my_half == iter) {
                MPI_Send(sendbuf, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
            }

            // Otherwise, receive
            else {        
                MPI_Recv(recvbuf, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);                
                *(int*) recvbuf = *(int*) recvbuf + *(int*) sendbuf;
            }
        }

        *(int*) sendbuf = *(int*) recvbuf;
    }

    // Temporary return value
    return 0;
}

int main() {

    // Initializing the MPI architecture

    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Generating the values we will sum together
    
    int value, sum;

    srand(time(NULL) * my_rank);
    value = (int) rand() % 100;
    sum = -1;

    printf("[%d] value: %d\n", my_rank, value);

    // Calling the default Allreduce function

    MPI_Allreduce(&value, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    printf("[%d] Default implementation: %d\n", my_rank, sum);

    // Calling our own Allreduce function

    if (comm_sz % 2 == 0) {
        MyMPI_Allreduce(&value, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        printf("[%d] Custom implementation: %d\n", my_rank, sum);
    }

    // Tearing down the MPI structures

    MPI_Finalize();

    return 0;
}
