#include "mpi.h"
#include <stdio.h>

// mpicc -o mpi_hello_world mpi_hello_world.c
// mpirun -np 4 ./mpi_hello_world 2>/dev/null

int main()
{
    int rank, nprocs;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    printf("Hello World\n");
    MPI_Finalize();
    return 0;
}