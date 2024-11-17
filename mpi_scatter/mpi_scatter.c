#include <mpi.h>
#include <stdio.h>

#define TAG 0
#define MASTER 0

// mpicc -o mpi_scatter mpi_scatter.c
// mpirun -np 4 ./mpi_scatter 2>/dev/null

int main(int argc, char **argv) {
    int rank, size;
    int vector[4];
    int element;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == MASTER) {
        for (int i = 0; i < 4; i++) {
            vector[i] = i + 1; // Inicializa com valores 1, 2, 3, 4
        }
    }

    MPI_Scatter(vector, 1, MPI_INT, &element, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Processo %d recebeu o elemento: %d\n", rank, element);

    MPI_Finalize();

    return 0;
}
