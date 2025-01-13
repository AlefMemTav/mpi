#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define MASTER 0

int main(int argc, char* argv[]) {
    int rank, nprocs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    if (rank == MASTER) {
        printf("Tamanho do vetor: ");
        fflush(stdout);
    }
    
    int tamvet;
    MPI_Bcast(&tamvet, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    
    // Calcula o tamanho do chunk local para cada processo
    int chunk = tamvet/nprocs;
    int resto = tamvet % nprocs;
    int local_size = chunk + (rank < resto ? 1 : 0);
    
    // Aloca apenas a memória necessária para o chunk local
    int *local_v = (int *) malloc(local_size * sizeof(int));
    
    // Apenas o MASTER aloca e inicializa o vetor completo
    int *v = NULL;
    if (rank == MASTER) {
        v = (int *) malloc(tamvet * sizeof(int));
        for (int i=0; i < tamvet; i++) {
            v[i] = i*2;  // Mantendo a mesma inicialização do original
        }
    }
    
    // Calcula os deslocamentos e tamanhos para cada processo
    int *sendcounts = (int *) malloc(nprocs * sizeof(int));
    int *displs = (int *) malloc(nprocs * sizeof(int));
    int disp = 0;
    
    for (int i = 0; i < nprocs; i++) {
        sendcounts[i] = chunk;
        if (i < resto) sendcounts[i]++;
        displs[i] = disp;
        disp += sendcounts[i];
    }
    
    // Distribui os dados usando MPI_Scatterv
    MPI_Scatterv(v, sendcounts, displs, MPI_INT,
                 local_v, local_size, MPI_INT,
                 MASTER, MPI_COMM_WORLD);
    
    // Imprime os elementos locais
    printf("%d/%d: ", rank, nprocs);
    for (int i=0; i < local_size; i++) {
        printf("%d ", local_v[i]);
    }
    printf("\n");
    
    // Libera a memória
    free(local_v);
    free(sendcounts);
    free(displs);
    if (rank == MASTER) {
        free(v);
    }
    
    MPI_Finalize();
    return 0;
}