#include <stdio.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define TAG 0

// mpicc -o mpi02 mpi02.c
// mpirun -np 4 ./mpi02

int main()
{
    int rank, nprocs, valor, resultado;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank == MASTER)
    {
        valor = 8;

        MPI_Send(&valor, 1, MPI_INT, SLAVE, TAG, MPI_COMM_WORLD);
        MPI_Recv(&resultado, 1, MPI_INT, SLAVE, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Master enviou %d e recebeu do Slave o valor %d\n", valor, resultado);
    }
    else
    {
        MPI_Recv(&valor, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        resultado = valor * 3;
        MPI_Send(&resultado, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}