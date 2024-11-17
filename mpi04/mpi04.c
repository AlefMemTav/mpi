#include <stdio.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define TAG 0
#define TAMVET 5

// mpicc -o mpi04 mpi04.c
// mpirun -np 4 ./mpi04

int main()
{
    int rank, nprocs, enviado;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank == MASTER)
    {
        int base = 8;

        for (int i = 1; i < nprocs; i++)
        {
            enviado = base * i;
            MPI_Send(&enviado, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(&enviado, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Slave (%d/%d) --> %d\n", rank, nprocs, enviado);
    }

    MPI_Finalize();
    return 0;
}