#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define R 4096
#define C 4096
#define BLOCK_W 32
#define BLOCK_H 32

float *a, *b, *result;

__global__ void multiple(float *A, float *B, float *res){
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    float ret = 0;

    for (int i = 0; i < C; i += BLOCK_W){
        for (int j = i; j < i + BLOCK_W; j++){
            ret += A[row * C + j] * B[j * C + col];
        }
    }

    res[row * C + col] = ret;
}

int main(){
  float *d_a, *d_b, *d_r;
  cudaEvent_t start, end;
  float etime;
  dim3 block(BLOCK_W, BLOCK_H);
  dim3 grid(C/BLOCK_W, R/BLOCK_H);

  a = (float*)malloc(sizeof(float)*R*C);
  b = (float*)malloc(sizeof(float)*R*C);
  result = (float*)malloc(sizeof(float)*R*C);

  cudaEventCreate(&start);
	cudaEventCreate(&end);
	srand(time(NULL));

  // random number creation
	for (int i = 0; i < R; i++)
    for (int j = 0; j < C; j++){
       a[i * C + j] = (rand() % 1000000) / (float)10000;
       b[i * C + j] = (rand() % 1000000) / (float)10000;
    }

  // cuda var initialization
  cudaMalloc((void**)&d_a, sizeof(float)*R*C);
  cudaMalloc((void**)&d_b, sizeof(float)*R*C);
  cudaMalloc((void**)&d_r, sizeof(float)*R*C);
  

  // kernel call & exec time check
  cudaEventRecord(start, 0);
  cudaMemcpy(d_a, a, sizeof(float)*R*C, cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, b, sizeof(float)*R*C, cudaMemcpyHostToDevice);
	multiple<<<grid, block>>>(d_a, d_b, d_r);
  cudaMemcpy(result, d_r, sizeof(float)*R*C, cudaMemcpyDeviceToHost);
	cudaEventRecord(end, 0);
	cudaEventSynchronize(end);
	cudaEventElapsedTime(&etime, start, end);

	printf("EXEC TIME : %f ms\n", etime);

  cudaEventDestroy(start);
	cudaEventDestroy(end);
  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_r);

	return 0;
}