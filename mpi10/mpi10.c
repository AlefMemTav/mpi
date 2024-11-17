#include <stdio.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define TAG 0
#define MAX 1000

// mpicc -o mpi10 mpi10.c
// mpirun -np 4 ./mpi09

// corrigir

int main(int argc, char *argv[])
{
    chunk = MAX/nprocs;
    ini = rank + chunk;
    if(rank == (nprocs - 1))
        fim += (MAX%nprocs);
    printf("Host %s, Processo %d/%d: ", host, rank, nprocs);
        for(int i = ini; i < fim; i++)
    
    MPI_Finalize();
    return 0;
}