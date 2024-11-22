#include <stdio.h>
#include <mpi.h>

// Reading from stdin the variables needed for the integral and sharing
// them to all other processes, since only one can access the input buffer
void get_input(int my_rank, int comm_sz, double *a_p, double *b_p, int *n_p) {
    
    // Iterate over all processes
    int destination;

    // Read from stdin and share the values to other processes
    if (my_rank == 0) {
        printf("Enter a, b, and n\n");
        scanf("%lf %lf %d", a_p, b_p, n_p);

        for (destination = 1; destination < comm_sz; destination++) {
            MPI_Send(a_p, 1, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD);
            MPI_Send(b_p, 1, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD);
            MPI_Send(n_p, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
        }
    }
    // Receive the values from the main process
    else {
        MPI_Recv(a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

// Same as before, same issues, but slightly more efficient
void get_input_bcast(int my_rank, int comm_sz, double *a_p, double *b_p, int *n_p) {

    // Read from stdin and share the values to other processes
    if (my_rank == 0) {
        printf("Enter a, b, and n\n");
        scanf("%lf %lf %d", a_p, b_p, n_p);
    }

    MPI_Bcast(a_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

// Change the function we are integrating. Current: f(x) = x^2
double f(double x) {
    return x * x;
}

// Compute the approxiate integral in the given sub-interval
double trap(double a, double b, double n, double h) {
    
    double estimate, x;
    int i;

    estimate = (f(a) + f(b)) / 2.0;

    for (i = 1; i <= n - 1; i++) {
        x = a + i * h;
        estimate += f(x);
    }

    estimate *= h;

    return estimate;
}

int main() {

    // MPI variables
    int my_rank;
    int comm_sz;

    // Variables to compute the sub-integral
    int n = 1024, local_n;
    double a = 0.0, b = 3000.0, h, local_a, local_b;
    
    // Constant initialization
    n = 1024;
    a = 0.0;
    b = 3000.0;

    /*
    // Reading from standard input the values
    // (needs to be adapted for a cluster, needs input validation)
    get_input(my_rank, comm_sz, &a, &b, &n);
    */

    // Variables to store the results
    double local_int, total_int;
    
    // Iterate over each process
    int source;

    // MPI commands
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // The height does not change
    // The local_n changes for each process
    h = (b - a) / n;
    local_n = n / comm_sz;

    // Computing the integral for this sub-interval
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    local_int = trap(local_a, local_b, local_n, h);

    // Sending the result to the main process
    if (my_rank != 0) {
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    // Computing the sum of the sub-integrals
    else {
        total_int = local_int;

        for (source = 1; source < comm_sz; source++) {
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }
    }

    // Printing the results
    if (my_rank == 0) {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
    }

    // Closing the MPI structures
    MPI_Finalize();

    return 0;
}
