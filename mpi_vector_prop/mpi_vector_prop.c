#include <mpi.h>
#include <stdio.h>

#define MASTER 0
#define TAMVEC 8

// mpicc -o mpi_vector_prop mpi_vector_prop.c
// mpirun -np 2 ./mpi_vector_prop

int main(int argc, char **argv)
{
    int rank, size;
    int vector[TAMVEC] = {1, 2, 3, 4, 5, 6, 7, 8};
    int chunk, start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunk = TAMVEC / size;
    start = rank * chunk;
    end = start + chunk;

    printf("Processo %d recebeu os elementos: ", rank);
    for (int i = start; i < end; i++)
    {
        printf("%d ", vector[i]);
    }
    printf("\n");

    MPI_Finalize();

    return 0;
}
