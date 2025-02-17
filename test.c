#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <mpi.h>
#include <string.h>
#include <omp.h>



#define CUDA_CHECK(cmd) do {                         \
    cudaError_t e = cmd;                              \
    if( e != cudaSuccess ) {                          \
      printf("Failed: Cuda error %s:%d '%s'\n",             \
          __FILE__,__LINE__,cudaGetErrorString(e));   \
      exit(EXIT_FAILURE);                             \
    }                                                 \
} while(0)
  
  
#define NCCL_CHECK(cmd) do {                         \
  ncclResult_t r = cmd;                             \
  if (r!= ncclSuccess) {                            \
    printf("Failed, NCCL error %s:%d '%s'\n",             \
        __FILE__,__LINE__,ncclGetErrorString(r));   \
    exit(EXIT_FAILURE);                             \
  }                                                 \
} while(0)


int main(int argc, char *argv[])
{
    int rank, size;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);\
    MPI_Get_processor_name(hostname, &name_len);

    // Allocate GPU memory
    int data_send = rank;
    int data_recv = -1;

    int rank_send = (rank + 1) % size;
    int rank_recv = (rank - 1 + size) % size;
    // Ping-pong logic
    int buff_start[10];
    #pragma omp parallel for
    for (int i = 0; i < 10; i++) {
        buff_start[i] = rank;
    }

    if(rank % 2 == 0){
        MPI_Send(&data_send, 1, MPI_INT, rank_send, 0, MPI_COMM_WORLD);
        MPI_Recv(&data_recv, 1, MPI_INT, rank_recv, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&data_recv, 1, MPI_INT, rank_recv, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&data_send, 1, MPI_INT, rank_send, 0, MPI_COMM_WORLD);
    }

        
    if(rank_recv != data_recv){
        printf("Some problems during computation!\n");
        return 1;
    }

    if(rank == 0)
        printf("Everything is fine! You are ready to go.\n");

    MPI_Finalize();

    return 0;
}
