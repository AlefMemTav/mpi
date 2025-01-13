#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX 1000
#define TEXT_SIZE 62
#define NUM_PROCESSES 3

// mpicc -o mpi_prova1 mpi_prova1.c
// mpirun -np 3 ./mpi_prova1 2>/dev/null

int main(int argc, char **argv)
{
    int rank, size, number, tmp_index, i, cont = 0;
    char texto_base[] = "abcdefghijklmnopqrstuvwxyz 1234567890 ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int *indice = (int *)malloc(sizeof(int));
    *indice = 0;
    struct timeval tv;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    while (cont < MAX)
    {
        gettimeofday(&tv, NULL);
        number = ((tv.tv_usec / 47) % 3) + 1;

        tmp_index = *indice + (rank * TEXT_SIZE / NUM_PROCESSES);

        for (i = 0; i < number; i++)
        {
            if (tmp_index + i < TEXT_SIZE)
            {
                fprintf(stderr, "%c", texto_base[tmp_index + i]);
            }
        }

        *indice = tmp_index + i;

        if (*indice >= TEXT_SIZE)
        {
            fprintf(stderr, "\n");
            *indice = 0;
        }

        cont++;
    }

    printf("\n");

    MPI_Finalize();
    return 0;
}