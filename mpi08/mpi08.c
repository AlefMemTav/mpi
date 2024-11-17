#include <stdio.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define TAG 0

// mpicc -o mpi08 mpi08.c
// mpirun -np 5 ./mpi08

int main()
{
    int rank, nprocs;
    int token = 1;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if(rank == MASTER) {
        printf("Master enviando token para o processo 1\n");
        MPI_Send(&token, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
        MPI_Recv(&token, 1, MPI_INT, (nprocs - 1), MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&token, 1, MPI_INT, (rank - 1), MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Recebido token de %d\n",(rank - 1));
        MPI_Send(&token, 1, MPI_INT, ((rank +1) % nprocs), TAG, MPI_COMM_WORLD); 
    }
    MPI_Finalize();
    return 0;
}