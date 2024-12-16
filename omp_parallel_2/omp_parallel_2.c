#include <stdio.h>
#include <omp.h>
#define TAM 12

int main() {
    int A[TAM], B[TAM], C[TAM];
    int i;

    for (i = 0; i < TAM; i++) {
        A[i] = 2 * i - 1;
        B[i] = i + 2;
    }

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int chunk_size = (TAM + num_threads - 1) / num_threads;
        int start = tid * chunk_size;
        int end = (start + chunk_size > TAM) ? TAM : start + chunk_size;

        for (i = start; i < end; i++) {
            C[i] = A[i] + B[i];
            printf("Thread[%d] calculou C[%d]\n", tid, i);
        }
    }

    for (i = 0; i < TAM; i++) {
        printf("C[%d] = %d\n", i, C[i]);
    }

    return 0;
}
