#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// mpicc -o mpi_master_init_slave_print mpi_master_init_slave_print.c
// mpirun -np 4 ./mpi_master_init_slave_print

// Elabore um programa MPI com N processos, sendo o master o responsável por inicializar o vetor e os slaves, responsáveis por
// imprimir uma porção do vetor, proporcional ao número de slaves identificados pelo programa.

imprimir uma porção do vetor, proporcional ao número de slaves identificados pelo programa.int main(int argc, char **argv)
{
    int rank, size;
    int *vector = NULL;
    int vector_size = 100;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_slaves = size - 1;

    if (rank == 0)
    {
        vector = (int *)malloc(vector_size * sizeof(int));
        for (int i = 0; i < vector_size; i++)
        {
            vector[i] = i + 1;
        }

        int base_chunk_size = vector_size / num_slaves;
        int remainder = vector_size % num_slaves;

        for (int i = 1; i <= num_slaves; i++)
        {
            int start_idx = (i - 1) * base_chunk_size + (i <= remainder ? i - 1 : remainder);
            int chunk_size = base_chunk_size + (i <= remainder ? 1 : 0);

            MPI_Send(&chunk_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&vector[start_idx], chunk_size, MPI_INT, i, 1, MPI_COMM_WORLD);
        }

        free(vector);
    }
    else
    {
        int chunk_size;
        MPI_Recv(&chunk_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int *chunk = (int *)malloc(chunk_size * sizeof(int));
        MPI_Recv(chunk, chunk_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Processo %d recebeu: ", rank);
        for (int i = 0; i < chunk_size; i++)
        {
            printf("%d ", chunk[i]);
        }
        printf("\n");

        free(chunk);
    }

    MPI_Finalize();
    return 0;
}
