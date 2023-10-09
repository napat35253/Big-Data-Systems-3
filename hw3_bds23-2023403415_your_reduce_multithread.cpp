#include "your_reduce.h"

// You may add your functions and variables here

void YOUR_Reduce(int const* sendbuf, int* recvbuf, int count) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Make a copy of the send buffer into the receive buffer to start
    memcpy(recvbuf, sendbuf, count * sizeof(int));

    for (int step = 1; step < size; step *= 2) {
        if ((rank % (2 * step)) == 0) {
            if (rank + step < size) {
                int *temp_buf = new int[count];
                MPI_Recv(temp_buf, count, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                
                #pragma omp parallel for schedule(static,1024)
                for (int i = 0; i < count; i++) {
                    recvbuf[i] += temp_buf[i];
                }

                delete[] temp_buf;
            }
        } else if ((rank % step) == 0) {
            MPI_Send(recvbuf, count, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
            break;
        }
    }
}