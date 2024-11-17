#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define TAG 0
#define MAX 1000

// mpicc -o mpi09 mpi09.c
// mpirun -np 4 ./mpi09

int main(int argc, char *argv[])
{
    int rank, nprocs;
    char host[20];
    int dados[4], recebido;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    gethostname(host, 19);

    if(rank == MASTER){
        dados[0] = 39;
        dados[1] = 42;
        dados[2] = 75;
        dados[3] = 128;  
    }

    MPI_Scatter(dados, 1, MPI_INT, &recebido, 1, MPI_INT, MASTER, MPI_COMM_WORLD); 
    printf("host %s processo %d/%d imprime valor %d\n", host, rank, nprocs, recebido);
    
    MPI_Finalize();
    return 0;

}