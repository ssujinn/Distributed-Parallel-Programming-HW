#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define R 4096
#define C 4096
#define BLOCK_W 32
#define BLOCK_H 32
#define SHARED_SIZE (BLOCK_W*BLOCK_H*4)
#define NSTREAM 1

float *a, *b, *result;

__global__ void multiple(float *A, float *B, float *res){
    __shared__ float shared_A[BLOCK_W * BLOCK_H];
    __shared__ float shared_B[BLOCK_W * BLOCK_H];
    int tx = threadIdx.x, ty = threadIdx.y;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    float ret = 0;

    for (int k = 0; k < (C - 1) / BLOCK_W + 1; k++){
        if (row < R && k * BLOCK_W + tx < C)
            shared_A[ty * BLOCK_W + tx] = A[row * C + k * BLOCK_W + tx];
        else
            shared_A[ty * BLOCK_W + tx] = 0;

        if (col < C && k * BLOCK_H + ty < R)
            shared_B[ty * BLOCK_W + tx] = B[(k * BLOCK_W + ty) * C + col];
        else
            shared_B[ty * BLOCK_W + tx] = 0;

        __syncthreads();

        for (int i = 0; i < BLOCK_W; i++){
          ret += shared_A[ty * BLOCK_W + i] * shared_B[i * BLOCK_W + tx];
        }
        __syncthreads();
    }

    if (row < C && col < C)
        res[row * C + col] = ret;
}

int main(){
  float *d_a, *d_b, *d_r;
  cudaEvent_t start, end;
  float etime;
  dim3 block(BLOCK_W, BLOCK_H);
  dim3 grid(C/BLOCK_W, R/BLOCK_H);

  cudaStream_t stream[NSTREAM];
  for (int i = 0 ; i < NSTREAM; i++)
    cudaStreamCreate(&stream[i]);

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
  cudaMemcpy(d_b, b, sizeof(float)*R*C, cudaMemcpyHostToDevice);
  for (int i = 0; i < NSTREAM; i++){
      int ELES_PER_STREAM = R * C / NSTREAM;
      int offset = i * ELES_PER_STREAM;
      cudaMemcpyAsync(&d_a[offset], &a[offset], sizeof(float)*ELES_PER_STREAM, cudaMemcpyHostToDevice, stream[i]);
      multiple<<<grid, block, 2 * SHARED_SIZE, stream[i]>>>(d_a + offset, d_b, d_r + offset);
      cudaMemcpyAsync(&result[offset], &d_r[offset], sizeof(float)*ELES_PER_STREAM, cudaMemcpyDeviceToHost, stream[i]);
  }

  for (int i = 0; i < NSTREAM; i++)
    cudaStreamSynchronize(stream[i]);
  
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