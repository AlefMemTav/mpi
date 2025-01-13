#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MASTER 0
#define TAG 1

int main(int argc, char **argv) {
    int rank, size, token, max;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == MASTER) {
        printf("Digite o valor máximo: ");
        scanf("%d", &max);

        for(int i = 1; i < size; i++) {
            MPI_Send(&max, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }
        token = 0;
        MPI_Send(&token, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
        printf("Processo %d/%d token=%d \n", rank, size, token);
    } else {
        MPI_Recv(&max, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    while (1) {
        MPI_Recv(&token, 1, MPI_INT, (rank - 1 + size) % size, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(token == max) {
            MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, TAG, MPI_COMM_WORLD);
            break;
        }
        token++;
        printf("Processo %d/%d token=%d \n", rank, size, token);
        usleep(2000);
        MPI_Send(&token, 1, MPI_INT, (rank + 1) % size, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}