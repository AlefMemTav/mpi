#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    int *vector = NULL;
    int *local_vector = NULL;
    int vector_size = 1000000; // Tamanho total do vetor
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Calcular o tamanho local para cada processo
    int local_size = vector_size / size;
    int remainder = vector_size % size;
    
    // Ajustar tamanho local se houver resto
    if (rank < remainder) {
        local_size++;
    }
    
    // Alocar apenas a parte local necessária
    local_vector = (int*)malloc(local_size * sizeof(int));
    
    // Apenas o MASTER aloca e inicializa o vetor completo
    if (rank == 0) {
        vector = (int*)malloc(vector_size * sizeof(int));
        // Inicialização do vetor
        for (int i = 0; i < vector_size; i++) {
            vector[i] = i;
        }
    }
    
    // Calcular os deslocamentos para cada processo
    int *sendcounts = (int*)malloc(size * sizeof(int));
    int *displs = (int*)malloc(size * sizeof(int));
    int disp = 0;
    
    for (int i = 0; i < size; i++) {
        sendcounts[i] = vector_size / size;
        if (i < remainder) {
            sendcounts[i]++;
        }
        displs[i] = disp;
        disp += sendcounts[i];
    }
    
    // Distribuir os dados usando MPI_Scatterv
    MPI_Scatterv(vector, sendcounts, displs, MPI_INT,
                 local_vector, local_size, MPI_INT,
                 0, MPI_COMM_WORLD);
    
    // Imprimir elementos locais
    printf("Processo %d: Primeiros 3 elementos: ", rank);
    for (int i = 0; i < (local_size < 3 ? local_size : 3); i++) {
        printf("%d ", local_vector[i]);
    }
    printf("\n");
    
    // Liberar memória
    free(local_vector);
    free(sendcounts);
    free(displs);
    if (rank == 0) {
        free(vector);
    }
    
    MPI_Finalize();
    return 0;
}