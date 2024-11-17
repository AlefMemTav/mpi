#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define TAG 99

// mpicc -o mpi_send_recv mpi_send_recv.c
// mpirun -np 4 ./mpi_send_recv

int main()
{
    int rank, nprocs;
    char msn[20];

    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {
        strcpy(msn, "Hello World");
        for (int i = 1; i < nprocs; i++)
        {
            MPI_Send(msn, 12, MPI_CHAR, i, TAG, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(msn, 12, MPI_CHAR, MPI_ANY_SOURCE, TAG, MPI_COMM_WORLD, &status);
    }

    printf("Message from node= %d : %.13s\n", rank, msn);
    MPI_Finalize();
    return 0;
}