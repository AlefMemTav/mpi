#include <mpi.h>
#include <stdio.h>

#define MASTER 0
#define TAMV 8

// mpicc -o mpi_master_vector_prop mpi_master_vector_prop.c
// mpirun -np 2 ./mpi_master_vector_prop

int main(int argc, char **argv)
{
    int rank, size, chunk, start, end, r;
    int vector[TAMV];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    chunk = TAMV / size;
    r = TAMV % size;

    if (rank < r)
    {
        start = rank * (chunk + 1);
        end = start + chunk + 1;
    }
    else
    {
        start = rank * chunk + r;
        end = start + chunk;
    }

    if (rank == MASTER)
    {
        for (int i = 0; i < TAMV; i++)
        {
            vector[i] = i + 1;
        }
    }

    MPI_Bcast(vector, TAMV, MPI_INT, MASTER, MPI_COMM_WORLD);

    printf("Processo %d imprimiu: ", rank);
    for (int i = start; i < end; i++)
    {
        printf("%d ", vector[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}