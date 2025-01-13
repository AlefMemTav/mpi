#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MASTER 0

// mpicc -o mpi_bcast mpi_bcast.c
// mpirun -np 4 ./mpi_bcast 2>/dev/null

int main(int argc, char **argv)
{
    int rank, size;
    int *vector = NULL;
    int vector_size;
    int *local_vector = NULL;
    int local_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == MASTER)
    {
        srand(time(NULL));
        vector_size = 8 + rand() % 20;

        vector = (int *)malloc(vector_size * sizeof(int));
        for (int i = 0; i < vector_size; i++)
        {
            vector[i] = rand() % 100;
        }
    }

    MPI_Bcast(&vector_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    local_size = vector_size / size;
    int remainder = vector_size % size;
    if (rank < remainder)
    {
        local_size++;
    }

    local_vector = (int *)malloc(local_size * sizeof(int));

    int *sendcounts = NULL;
    int *displs = NULL;
    if (rank == MASTER)
    {
        sendcounts = (int *)malloc(size * sizeof(int));
        displs = (int *)malloc(size * sizeof(int));

        int offset = 0;
        for (int i = 0; i < size; i++)
        {
            sendcounts[i] = vector_size / size + (i < remainder ? 1 : 0);
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    MPI_Scatterv(vector, sendcounts, displs, MPI_INT, local_vector, local_size, MPI_INT, MASTER, MPI_COMM_WORLD);

    printf("Processo %d recebeu %d elementos: ", rank, local_size);
    for (int i = 0; i < local_size; i++)
    {
        printf("%d ", local_vector[i]);
    }
    printf("\n");

    free(local_vector);
    if (rank == MASTER)
    {
        free(vector);
        free(sendcounts);
        free(displs);
    }

    MPI_Finalize();
    return 0;
}
