#include <stdio.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define TAG 0

// mpicc -o mpi01 mpi01.c
// mpirun -np 4 ./mpi01

int main()
{
    int rank, nprocs;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank == MASTER)
    {
        int i = 8;

        MPI_Send(&i, 1, MPI_INT, SLAVE, TAG, MPI_COMM_WORLD);
    }
    else
    {
        int rec;
        MPI_Recv(&rec, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("SLAVE (%d/%d) recebeu %d\n", rank, nprocs, rec);
    }

    MPI_Finalize();
    return 0;
}