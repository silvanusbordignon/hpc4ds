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

    // Actual implementation

    int height, group_sz, group_leader_rank, old_group_sz, old_group_leader_rank, source, target;
    int local_sum = *((int *) sendbuf);

    /*
        APPROACH (idea by ChatGPT): have some processes first receive then send, based on their
        position w.r.t. the group
    */

    for (height = 0; height <= log2(comm_sz); height++) {
        
        old_group_sz = (int) pow(2, height);
        old_group_leader_rank = my_rank - my_rank % old_group_sz;
        source = old_group_leader_rank + ((my_rank + (int) pow(2, height - 1)) % old_group_sz);

        group_sz = (int) pow(2, height + 1);
        group_leader_rank = my_rank - my_rank % group_sz;
        target = group_leader_rank + ((my_rank + (int) pow(2, height)) % group_sz);

        // printf("[%d] Height: %d, source: %d, target: %d\n", my_rank, height, source, target);

        // First iteration: only send
        if (height == 0) {

            // printf("[%d][h=%d] sending to %d\n", my_rank, height, target);
            MPI_Send(&local_sum, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
        }
        // Last iteration: only receive
        else if (height == log2(comm_sz)) {
            
            // printf("[%d][h=%d] receiving from %d\n", my_rank, height, source);
            MPI_Recv(&recvbuf, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            *(int*) recvbuf = *(int*) recvbuf + local_sum;
        }
        // Intermediate iterations: receive and send
        else {

            // printf("[%d][h=%d] receiving from %d\n", my_rank, height, source);
            MPI_Recv(&recvbuf, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_sum = local_sum + *(int *) recvbuf;

            // printf("[%d][h=%d] sending to %d\n", my_rank, height, target);
            MPI_Send(&local_sum, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
        }
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
