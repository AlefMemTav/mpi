#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int *array;
    int array_size;
    int num_threads;

    srand((unsigned)time(NULL));
    array_size = (rand() % 1000) + 1;
    array = (int*)malloc(array_size * sizeof(int));

    for (int i = 0; i < array_size; i++) {
        array[i] = rand() % 100;
    }

    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }

    int *count_per_thread = (int*)calloc(num_threads, sizeof(int));

    #pragma omp parallel for
    for (int i = 0; i < array_size; i++) {
        int thread_id = omp_get_thread_num();
        count_per_thread[thread_id]++;
    }

    printf("Vetor de entrada com tamanho: %d\n", array_size);
    printf("Distribuição do trabalho por thread:\n");
    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d: %d índices\n", i, count_per_thread[i]);
    }

    free(array);
    free(count_per_thread);

    return 0;
}
