#include <cuda.h>
#include <iostream>
#include <cstdlib>

using namespace std;

#define THREADSPERBLOCK 1024

// __global__ void mandel(char *d_vet, int MAX_N, int MAX_COL, int MAX_ROW) {
__global__ void mandel(char *d_vet, int MAX_ROW, int MAX_COL, int MAX_NUM) {

	int i = blockIdx.x * blockDim.x + threadIdx.x;

	if (i < MAX_ROW * MAX_COL) {

		int c = i % MAX_COL;
		int r = i / MAX_COL;
		int n = 0;

		float x=0, y=0, tmp;
		while ((x*x) + (y*y) < 4 && ++n < MAX_NUM) {
			tmp = x*x - y*y + ((float)c * 2 / MAX_COL - 1.5);
			y   = x * y * 2 + ((float)r * 2 / MAX_ROW - 1);
			x   = tmp;
		}

		d_vet[i] = (n == MAX_NUM ? '#' : '.');
	}
}

int main(int argc, char **argv) {

	int MAX_ROW;
	int MAX_COL;
	int MAX_NUM;

	if (argc == 4) {
		MAX_ROW = atoi(argv[1]);
		MAX_COL = atoi(argv[2]);
		MAX_NUM = atoi(argv[3]);
	}
	else
		exit(1);

	int MAX_TAM = MAX_ROW * MAX_COL;

	size_t i, size = MAX_TAM * sizeof(char);
	int nBlocks = ( MAX_TAM + THREADSPERBLOCK - 1 ) / THREADSPERBLOCK;

	char *h_vet = (char *)malloc(size);
	char *d_vet = (char *)malloc(size);

	cudaSetDevice(0);

	cudaMalloc((void**)&d_vet, size);

	mandel<<<nBlocks, THREADSPERBLOCK>>>(d_vet, MAX_ROW, MAX_COL, MAX_NUM);

	cudaMemcpy(h_vet, d_vet, size, cudaMemcpyDeviceToHost);
	
	cudaFree(d_vet);

	for(i = 0; i < MAX_TAM; ++i) {
		cout << h_vet[i];
		if (i % MAX_COL == MAX_COL-1)
			cout << endl;
	}

	free(h_vet);
}
