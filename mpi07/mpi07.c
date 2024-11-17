#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define TAG 0
#define MAX 1000

// mpicc -o mpi07 mpi07.c
// mpirun -np 4 ./mpi07

int main()
{
    int rank, nprocs;
    char host[100];
    char msg[200];
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank != MASTER)
    {
        gethostname(host, 19);
        sprintf(msg, "Rodando em %s com valor %d", host, (MAX + rank));
        MPI_Send(&msg, 200, MPI_CHAR, MASTER, TAG, MPI_COMM_WORLD);
    }
    else
    {
        for (int i = 1; i < nprocs; i++)
        {
            MPI_Recv(msg, 200, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Recebido mensagem %s do slave %d\n", msg, i);
        }
    }
    MPI_Finalize();
    return 0;
}