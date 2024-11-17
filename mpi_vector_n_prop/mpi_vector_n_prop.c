#include <mpi.h>
#include <stdio.h>

#define MASTER 0
#define TAMV 8

// mpicc -o mpi_vector_n_prop mpi_vector_n_prop.c
// mpirun -np 4 ./mpi_vector_n_prop

int main(int argc, char **argv)
{
    int rank, size;
    int vector[TAMV];
    int chunk;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    int local_vector[TAMV / size];

    if (rank == MASTER)
    {
        for (int i = 0; i < TAMV; i++)
        {
           vector[i] = i+1;
        }
    }

    chunk = TAMV / size;

    MPI_Scatter(vector, chunk, MPI_INT, local_vector, chunk, MPI_INT, MASTER, MPI_COMM_WORLD);

    printf("Processo %d recebeu: ", rank);
    for (int i = 0; i < chunk; i++)
    {
        printf("%d ", local_vector[i]);
    }
    printf("\n");

    MPI_Finalize();

    return 0;
}
