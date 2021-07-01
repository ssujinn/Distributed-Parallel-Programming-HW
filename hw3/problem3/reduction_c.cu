#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ELEMENT 100000
#define BLOCK_SIZE 128
#define GRID_SIZE ((NUM_ELEMENT + BLOCK_SIZE - 1) / BLOCK_SIZE)
#define WARP_SIZE 32

__global__ void reduction_max(int *Arr, int *Max){
    int id = blockIdx.x * blockDim.x + threadIdx.x;

    for (int i = 1; i < blockDim.x; i *= 2){
        if ((id / WARP_SIZE) % (2*i) == 0){
            if (Arr[id] < Arr[id + i])
              Arr[id] = Arr[id + i];
        }
        __syncthreads();
    }

    if (id == 0)
      Max[0] = Arr[0];
}

int main(){
	int* arr;
  int* d_arr, *d_max;
	int max = 0;
  cudaEvent_t start, end;
  float etime;
  dim3 block(BLOCK_SIZE);
  dim3 grid(GRID_SIZE);

  cudaEventCreate(&start);
	cudaEventCreate(&end);
	srand(time(NULL));

  // random number creation
	arr = (int*)malloc(sizeof(int) * NUM_ELEMENT);
	for (int i = 0; i < NUM_ELEMENT; i++)
		arr[i] = rand() % (NUM_ELEMENT * 10);

  // tmp print
	//for (int i = 0; i < NUM_ELEMENT; i++)
	//	printf("%d\n", arr[i]);

  // cuda var initialization
  cudaMalloc((void**)&d_arr, sizeof(int)*NUM_ELEMENT);
  cudaMalloc((void**)&d_max, sizeof(int));
  cudaMemcpy(d_arr, arr, sizeof(int)*NUM_ELEMENT, cudaMemcpyHostToDevice);

  // kernel call & exec time check
  cudaEventRecord(start, 0);
	reduction_max<<<grid, block>>>(d_arr, d_max);
	cudaEventRecord(end, 0);
	cudaEventSynchronize(end);
	cudaEventElapsedTime(&etime, start, end);

  cudaMemcpy(&max, d_max, sizeof(int), cudaMemcpyDeviceToHost);

	printf("MAX NUM : %d\n", max);
	printf("EXEC TIME : %f ms\n", etime);

  cudaEventDestroy(start);
	cudaEventDestroy(end);
  cudaFree(d_arr);
  cudaFree(d_max);

	return 0;
}