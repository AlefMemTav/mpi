#include <stdio.h>
#include <cuda.h>

// nvcc -o cuda_pi cuda_pi.cu
// ./cuda_pi

#define NUM_STEPS 8000000
#define THREADS_PER_BLOCK 256

__global__ void calculate_pi(double step, double* partial_sums, int num_steps) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = gridDim.x * blockDim.x;
    double x;
    double sum = 0.0;

    for (int i = idx; i < num_steps; i += stride) {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    partial_sums[idx] = sum;
}

int main() {
    double step = 1.0 / (double)NUM_STEPS;
    double *d_partial_sums, *h_partial_sums;
    double pi = 0.0;

    int num_threads = THREADS_PER_BLOCK;
    int num_blocks = (NUM_STEPS + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    size_t partial_sums_size = num_blocks * THREADS_PER_BLOCK * sizeof(double);

    // Alocar memória na CPU e GPU
    h_partial_sums = (double*)malloc(partial_sums_size);
    cudaMalloc(&d_partial_sums, partial_sums_size);

    // Lançar o kernel
    calculate_pi<<<num_blocks, num_threads>>>(step, d_partial_sums, NUM_STEPS);

    // Copiar resultados parciais de volta para a CPU
    cudaMemcpy(h_partial_sums, d_partial_sums, partial_sums_size, cudaMemcpyDeviceToHost);

    // Somar os resultados parciais
    for (int i = 0; i < num_blocks * THREADS_PER_BLOCK; i++) {
        pi += h_partial_sums[i];
    }

    pi *= step;

    // Limpar memória
    free(h_partial_sums);
    cudaFree(d_partial_sums);

    printf("Pi = %.15f\n", pi);
    return 0;
}
