// Note: when thread count goes up, the clock() function loses its usefulness
// since it measures CPU time, and if we use multiple CPUs their "use" time is
// summed

#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

// Current function: x^2... a bit slowed down
double f(double x) {

    for (int i = 0; i < 100; i++);
    return x * x;
}

// Compute the area of the function as the sum trapezoids
double local_trap(double a, double b, int n);

int main(int argc, char **argv) { 

    double  global_result = 0.0;    // store results
    double  a, b;                   // left and right endpoints
    int     n;                      // total number of trapezoids
    int     thread_count;

    thread_count = strtol(argv[1], NULL, 10);
    printf("Enter a, b, and n\n");
    scanf("%lf %lf %d", &a, &b, &n);

    // Starting a timer
    clock_t begin = clock();

    #pragma omp parallel num_threads(thread_count) reduction(+: global_result)
    global_result += local_trap(a, b, n);

    // Ending a timer and computing how much time has passed
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.14e\n", a, b, global_result);

    printf("\nThe computation took %lf seconds", time_spent);

    return 0;
}

double local_trap(double a, double b, int n) {
    
    double h, x, my_result;
    double local_a, local_b;
    int local_n;
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();

    h = (b - a) / n;
    local_n = n / thread_count;
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;
    my_result = (f(local_a) + f(local_b)) / 2.0;

    for (int i = 1; i <= local_n - 1; i++) {
        x = local_a + i * h;
        my_result += f(x);
    }
    my_result = my_result * h;

    return my_result;
}