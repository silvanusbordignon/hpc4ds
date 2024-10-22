/*


TODO


- matrice tridimensionale per il salvataggio dei risultati....
- ristruttura la funzione degli esperimenti che lancia 10 esperimenti per una data dimensione del vettore
- una volta fatto ciò, prepara le funzioni per la media, moda, mediana
- stampa le statistiche rilevanti e successivamente tutti i risultati da plottare




*/







#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

const long MAX_SIZE = 2147483647;
const int CYCLES = 10;

double compute_mean(double* array);
double compute_median(double* array);
double compute_mode(double* array);

double** run_cycles(int rank, int n);

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

    int exp_count = ceil(log2(MAX_SIZE));
    double** results = (double*) malloc(sizeof(double) * exp_count * 3);

    for (int i = 1; i <= MAX_SIZE; i*=2) {
        results[i] = run_cycles(my_rank, i);
    }

    MPI_Finalize();

    return 0;
}


double** run_cycles(int rank, int n) {

    if (rank == 0) {
        
        double t0, t1;

        double* array = (double*) malloc(sizeof(double) * n);
        double** results = (double*) malloc(sizeof(double) * CYCLES * 2);

        for (int i = 0; i < CYCLES; i++) {

            MPI_Barrier(MPI_COMM_WORLD);

            t0 = MPI_Wtime();

            MPI_Send(array, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(array, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            t1 = MPI_Wtime();

            double bandwidth = (((n * 8) / 1000000) / ((t1 - t0) / 2));
            
            results[0][i] = t1 - t0;
            results[1][i] = bandwidth;
        }

        double* stats = malloc(sizeof(double) * 3);

        double[0] = compute_mean(results[0])

        free(array);
        free(results);

        return stats;
    }
    else if (rank == 1) {

        double* array = (double*) malloc(sizeof(double) * n);

        for (int i = 0; i < CYCLES; i++) {

            MPI_Barrier(MPI_COMM_WORLD);

            MPI_Recv(array, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(array, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

        }
        
        free(array);
        return NULL;
    }
}


//--------

/*


TODO


- matrice tridimensionale per il salvataggio dei risultati....
- ristruttura la funzione degli esperimenti che lancia 10 esperimenti per una data dimensione del vettore
- una volta fatto ciò, prepara le funzioni per la media, moda, mediana
- stampa le statistiche rilevanti e successivamente tutti i risultati da plottare




*/







#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

const long MAX_SIZE = 2147483647;
const int CYCLES = 10;

int compute_digits(long value);
double** single_benchmark(int rank, int n);

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

    if (my_rank == 0) {
        
    }
    
    int exp_count = ceil(log2(MAX_SIZE));

    double* results = (double*) malloc(CYCLES * exp_count * 2 * sizeof(double));

    for (int i = 1; i <= M_CYCLES; i++) {
        single_benchmark(my_rank, i, results);
        printf("\n");
    }

    double mean = compute_mean();
    double median = compute_median();
    double mode = compute_mode();

    free(results);

    MPI_Finalize();

    return 0;
}

int compute_digits(long value) {
    
    int digits = 1;
    long temp = MAX_SIZE;

    while (temp / 10 > 0) {
        temp = (long) temp / 10;
        digits += 1;
    }

    return digits;
}

double** single_benchmark(int rank, int n) {

    double** results = NULL;

    if (rank == 0) {
        
        double t0, t1;

        double* array = (double*) malloc(sizeof(double) * n);
        double** results = (double*) malloc(sizeof(double) * 2 * CYCLES);

        for (int i = 0; i < CYCLES; i++) {

            MPI_Barrier(MPI_COMM_WORLD);

            t0 = MPI_Wtime();

            MPI_Send(array, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(array, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            t1 = MPI_Wtime();

            free(array);

            double bandwidth = (((n * 8) / 1000000) / ((t1 - t0) / 2));
            
            results[i]
        }
    }
    else if (rank == 1) {

        double* array = (double*) malloc(sizeof(double) * n);

        for (int i = 0; i < CYCLES; i++) {

            MPI_Barrier(MPI_COMM_WORLD);

            MPI_Recv(array, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(array, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

            free(array);
        }
    }
}

// .----

/*


TODO


- matrice tridimensionale per il salvataggio dei risultati....
- ristruttura la funzione degli esperimenti che lancia 10 esperimenti per una data dimensione del vettore
- una volta fatto ciò, prepara le funzioni per la media, moda, mediana
- stampa le statistiche rilevanti e successivamente tutti i risultati da plottare




*/







#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

const long MAX_SIZE = 2147483647;
const int CYCLES = 10;

int compute_digits(long value);
double** single_benchmark(int rank, int n);

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

    if (my_rank == 0) {
        
    }
    
    int exp_count = ceil(log2(MAX_SIZE));

    double* results = (double*) malloc(CYCLES * exp_count * 2 * sizeof(double));

    for (int i = 1; i <= M_CYCLES; i++) {
        single_benchmark(my_rank, i, results);
        printf("\n");
    }

    double mean = compute_mean();
    double median = compute_median();
    double mode = compute_mode();

    free(results);

    MPI_Finalize();

    return 0;
}

int compute_digits(long value) {
    
    int digits = 1;
    long temp = MAX_SIZE;

    while (temp / 10 > 0) {
        temp = (long) temp / 10;
        digits += 1;
    }

    return digits;
}

double** single_benchmark(int rank, int n) {

    double** results = NULL;

    if (rank == 0) {
        
        double t0, t1;

        double* array = (double*) malloc(sizeof(double) * n);
        double** results = (double*) malloc(sizeof(double) * 2 * CYCLES);

        for (int i = 0; i < CYCLES; i++) {

            MPI_Barrier(MPI_COMM_WORLD);

            t0 = MPI_Wtime();

            MPI_Send(array, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(array, n, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            t1 = MPI_Wtime();

            double bandwidth = (((n * 8) / 1000000) / ((t1 - t0) / 2));
            
            results[i][0] = t1 - t0;
            results[i][1] = bandwidth;
        }

        free(array);
        return results;
    }
    else if (rank == 1) {

        double* array = (double*) malloc(sizeof(double) * n);

        for (int i = 0; i < CYCLES; i++) {

            MPI_Barrier(MPI_COMM_WORLD);

            MPI_Recv(array, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(array, n, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

        }
        
        free(array);
        return
    }
}