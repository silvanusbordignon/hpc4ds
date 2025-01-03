#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

// Have the main process read a vector and split its
// content between all processes using MPI_Scatter
void read_vector(double local_a[], int local_n, int n, int my_rank, MPI_Comm comm) {
    
    double *a = NULL;
    int i;

    // The main process reads the vector, and to do so allocates some memory
    if (my_rank == 0) {

        a = malloc(n * sizeof(double));

        printf("Enter the vector:\n");
        for (i = 0; i < n; i++) {
            scanf("%lf", &a[i]);
        }

        // Distribute a piece of the array to each process, including myself
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
        
        // Since we end up with our slice of the array in local_a, here we free
        free(a);
    }
    else {
        // Receive a part of the array
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
    }
}

// Have all processes send back their values to the main one using 
// MPI_Gather and have the main process print the full array
void print_vector(double local_b[], int local_n, int n, int my_rank, MPI_Comm comm) {

    double *b = NULL;
    int i;

    // The main process receives the vector, and to do so allocates some memory
    if (my_rank == 0) {
        
        b = malloc(n * sizeof(double));

        // Receive all vector portions from the processes
        MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);

        printf("Vector:\n");
        for (i = 0; i < n; i++) {
            printf("%f ", b[i]);
        }
        printf("\n");

        // After we finished all our computations we can free the full vector
        free(b);
    }
    else {
        // Send my part of the array
        MPI_Gather(local_b, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, comm);
    }
}

// Serial version of a vector addition
void parallel_vector_sum(double local_x[], double local_y[], double local_z[], int local_n)
{
    int local_i;
    for (local_i = 0; local_i < local_n; local_i++) {
        local_z[local_i] = local_x[local_i] + local_y[local_i];
    }
}

int main(int argc, char **argv) {

    // MPI initialization and communicator instructions

    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Rudimental input sanitization

    if (my_rank == 0) {

        if (argc != 2) {
            printf("usage: %s [how many numbers per process]\n", argv[0]);
            
            MPI_Finalize();
            return 0;
        }

        if (strtol(argv[1], NULL, 10) < 1) {
            printf("command line argument must be positive\n");
            
            MPI_Finalize();
            return 0;
        }
    }


    // Declaring my variables
    
    int local_n = strtol(argv[1], NULL, 10);
    int n = local_n * comm_sz;

    double *local_x = malloc(local_n * sizeof(double));
    double *local_y = malloc(local_n * sizeof(double));
    double *local_z = malloc(local_n * sizeof(double));

    // After these calls, each process has a portion of the arrays
    read_vector(local_x, local_n, n, my_rank, MPI_COMM_WORLD);
    read_vector(local_y, local_n, n, my_rank, MPI_COMM_WORLD);

    // This call performs the sums of the two portions
    parallel_vector_sum(local_x, local_y, local_z, local_n);

    // With this final call we bring back the full, summed array and print it
    print_vector(local_z, local_n, n, my_rank, MPI_COMM_WORLD);


    MPI_Finalize();
    return 0;
}
