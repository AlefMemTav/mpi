#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MASTER 0
#define TAG 1

// mpicc -o mpi_token mpi_token.c
// mpirun -np 4 ./mpi_token 2>/dev/null

int main(int argc, char **argv)
{    
    int rank, size, token;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == MASTER)
    {
        token = 0;
        MPI_Send(&token, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
        printf("Processo %d/%d token=%d \n", rank, size, token);
    }
    else 
    {
        MPI_Recv(&token, 1, MPI_INT, (rank-1), TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        token++;
        printf("Processo %d/%d token=%d \n", rank, size, token);
        if(rank < (size -1))
        {
            MPI_Send(&token, 1, MPI_INT, (rank+1), TAG, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    return 0;
}
