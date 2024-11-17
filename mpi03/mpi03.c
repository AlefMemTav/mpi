#include <stdio.h>
#include <mpi.h>

#define MASTER 0
#define SLAVE 1
#define TAG 0
#define TAMVET 5

// mpicc -o mpi03 mpi03.c
// mpirun -np 4 ./mpi03

int main()
{
    int rank, nprocs;
    float media;
    int v[TAMVET];

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank == MASTER)
    {
        for (int i = 0; i < TAMVET; i++)
        {
            v[i] = 2 * i + 1;
        }

        MPI_Send(v, TAMVET, MPI_INT, SLAVE, TAG, MPI_COMM_WORLD);
        MPI_Recv(&media, 1, MPI_FLOAT, SLAVE, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Resultado = %f\n", media);
    }
    else
    {
        int soma = 0;

        MPI_Recv(v, TAMVET, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < TAMVET; i++)
        {
            soma += v[i];
        }

        media = (float)soma / TAMVET;

        MPI_Send(&media, 1, MPI_FLOAT, MASTER, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}