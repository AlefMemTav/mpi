#include <mpi.h>
#include <stdio.h>

#define TAG 0
#define MASTER 0

// mpicc -o mpi_vector_dist mpi_vector_dist.c
// mpirun -np 4 ./mpi_vector_dist 2>/dev/null

int main(int argc, char **argv)
{
    int rank, size;
    int element;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == MASTER)
    {
        int vector[4] = {1, 2, 3, 4};

        for (int i = 1; i < 4; i++)
        {
            MPI_Send(&vector[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }

        element = vector[0];
    }
    else
    {
        MPI_Recv(&element, 1, MPI_INT, MASTER, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    printf("Processo %d recebeu o elemento: %d\n", rank, element);

    MPI_Finalize();

    return 0;
}
