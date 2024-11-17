#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define WORKER1 1
#define WORKER2 2
#define N 4
#define TAG 0

// mpicc -o mpi_soma_matrizes mpi_soma_matrizes.c
// mpirun -np 4 ./mpi_soma_matrizes

// Utilizando a biblioteca MPI, elabore um programa para somar os elementos de duas matrizes A e B, quadradas (int), para gerar uma
// matriz C, de acordo com as seguintes regras:
// [x] O programa deve conter um processo master e dois workers, que deverão trabalhar em conjunto para garantir a realização de soma
// dos elementos das matrizes A e B
// [x] Supor que as matrizes sejam de 16 posições (int) e as matrizes A e B devem ser inicializadas com valores randômicos
// [x] As operações de soma devem ser distribuídas entre os workers, de modo que a primeira e a terceira linha das matrizes sejam
// processadas pelo primeiro worker e a segunda e a quarta linha sejam processadas pelo segundo worker
// [x] A comunicação entre os processos master e workers deve ser feita utilizando especificamente as funções MPI_Send e MPI_Recv
// [x] Ao final, a matriz C resultante deve ser impressa (em colunas, formato de matriz) pelo processo master

void initialize_matrix(int matrix[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

void print_matrix(int matrix[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    int rank, size;
    int A[N][N], B[N][N], C[N][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3)
    {
        if (rank == MASTER)
        {
            printf("O programa requer exatamente 3 processos (1 master e 2 workers).\n");
        }
        MPI_Finalize();
        exit(1);
    }

    if (rank == MASTER)
    {
        srand(42);
        initialize_matrix(A);
        initialize_matrix(B);

        for (int i = 0; i < N; i++)
        {
            if (i == 0 || i == 2)
            {
                MPI_Send(A[i], N, MPI_INT, WORKER1, TAG, MPI_COMM_WORLD);
                MPI_Send(B[i], N, MPI_INT, WORKER1, TAG, MPI_COMM_WORLD);
            }
            else
            {
                MPI_Send(A[i], N, MPI_INT, WORKER2, TAG, MPI_COMM_WORLD);
                MPI_Send(B[i], N, MPI_INT, WORKER2, TAG, MPI_COMM_WORLD);
            }
        }

        for (int i = 0; i < N; i++)
        {
            if (i == 0 || i == 2)
            {
                MPI_Recv(C[i], N, MPI_INT, WORKER1, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else
            {
                MPI_Recv(C[i], N, MPI_INT, WORKER2, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        printf("\nMatriz C (resultado da soma):\n");
        print_matrix(C);
    }
    else if (rank == WORKER1)
    {
        int A_row[N], B_row[N], C_row[N];
        for (int l = 0; l < 2; l++)
        {
            MPI_Recv(A_row, N, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(B_row, N, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < N; i++)
            {
                C_row[i] = A_row[i] + B_row[i];
            }

            MPI_Send(C_row, N, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
        }
    }
    else if (rank == WORKER2)
    {
        int A_row[N], B_row[N], C_row[N];

        for (int l = 0; l < 2; l++)
        {
            MPI_Recv(A_row, N, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(B_row, N, MPI_INT, MASTER, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int i = 0; i < N; i++)
            {
                C_row[i] = A_row[i] + B_row[i];
            }

            MPI_Send(C_row, N, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
