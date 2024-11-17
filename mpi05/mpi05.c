#include <stdio.h>
#include <mpi.h>

#define SLAVE 0
#define TAG 0

// mpicc -o mpi05 mpi05.c
// mpirun -np 4 ./mpi05

int main()
{

    int rank, nprocs, valor;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank == SLAVE)
    {
        int recebidos = 0;
        printf("Slave aguardando mensagens de %d masters...\n", nprocs - 2);

        for (int i = 1; i < nprocs; i++)
        {
            MPI_Recv(&valor, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Slave recebeu de Master %d: %d\n", i, valor);
            recebidos++;
        }

        printf("Slave recebeu um total de %d mensagens.\n", recebidos);
    }
    else
    {
        valor = rank * 10;
        MPI_Send(&valor, 1, MPI_INT, SLAVE, TAG, MPI_COMM_WORLD);
        printf("Master %d enviou %d\n", rank, valor);
    }
    MPI_Finalize();
    return 0;
}