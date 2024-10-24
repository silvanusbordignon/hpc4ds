#include <mpi.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <malloc.h>

const long MIN_SIZE = 134217728;
const long MAX_SIZE = 1073741824;
const long CYCLES = 25;

double compute_mean(double *array, int size);
double compute_median(double *array, int size);

int main() {

    int my_rank;
    int comm_sz;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    if (comm_sz != 2) {

        if (my_rank == 0) {
            fprintf(stderr, "For this benchmark we require two processes\n");
        }

        MPI_Finalize();
        return 1;
    }

    double *array;

    if (my_rank == 0) {

        double *times, *bandwidths;

        times = (double *) malloc(sizeof(double) * CYCLES);
        bandwidths = (double *) malloc(sizeof(double) * CYCLES);

        for (long array_size = MIN_SIZE; array_size <= MAX_SIZE; array_size *= 2) {
            
            printf("--------------------------------\n\n");
            printf("n: %ld\n\n", array_size);
            printf("time (sec)  Rate (MB/sec)\n");

            array = (double *) malloc(sizeof(double) * array_size);

            double t0, t1;
            double time, bandwidth;

            for (int i = 0; i < CYCLES; i++) {
                
                MPI_Barrier(MPI_COMM_WORLD);

                t0 = MPI_Wtime();

                MPI_Send(array, array_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(array, array_size, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
                t1 = MPI_Wtime();

                time = t1 - t0;
                bandwidth = (((array_size * 8) / 1000000) / ((t1 - t0) / 2));

                times[i] = time;
                bandwidths[i] = bandwidth;

                printf("%f    %f\n", time, bandwidth);
            }

            printf("\n");
            printf("Mean time: %lf sec\n", compute_mean(times, CYCLES));
            printf("Median time: %lf sec\n", compute_median(times, CYCLES));
            printf("\n");
            printf("Mean bandwidth: %lf MB/sec\n", compute_mean(bandwidths, CYCLES));
            printf("Median bandwidth: %lf MB/sec\n", compute_median(bandwidths, CYCLES));
            printf("\n");

            free(array);
        }

        free(times);
        free(bandwidths);
    }
    else if (my_rank == 1) {
        for (long array_size = MIN_SIZE; array_size <= MAX_SIZE; array_size *= 2) {
            
            array = (double *) malloc(sizeof(double) * array_size);

            for (int i = 0; i < CYCLES; i++) {
                
                MPI_Barrier(MPI_COMM_WORLD);

                MPI_Recv(array, array_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(array, array_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }

            free(array);
        }
    }

    MPI_Finalize();

    return 0;
}


double compute_mean(double *array, int size)
{
    double sum = 0;

    for (int i = 0; i < size; i++) {
        sum += array[i];
    }

    return (sum != 0) ? (sum / size) : 0;
}
double compute_median(double *array, int size)
{
    double temp;
    int min;

    for (int i = 0; i < size - 1; i++) {

        min = i;

        for (int j = 1; j < size; j++) {
            if (array[j] < array[min]) {
                temp = array[j];
                array[j] = array[min];
                array[min] = temp;
            }
        }
    }

    if (size % 2 == 0) {
        return (array[size / 2 - 1] + array[size / 2]) / 2;
    }
    else {
        return array[size / 2];
    }
}