#include <qsml.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum {
  ROWS = 8192,
  COLUMNS = 8192};

/*
 * In order to calculate the product of these two matrix you need
 * 8192 products and 8193 adds per element
 * (8192 + 8193) * 8192 = 134.225.920 floating point operations
 * But we do the test 10 times
 */

static const double FLOP = 1342259200.0;


int main(int argc, char **argv)
{
	uint32_t matrixSize = ROWS*COLUMNS;

	float *A = new float[matrixSize];
	float *B = new float[matrixSize];
	float *C = new float[matrixSize];

	printf("Filling matrix\n");
	for (uint32_t i=0; i<matrixSize;i++) {
		A[i]=(float)rand()/(float)(RAND_MAX/1000);
		B[i]=(float)rand()/(float)(RAND_MAX/1000);
	}


	printf("Starting test...\n");
	
	for (int i=0; i<10; i++) {
		clock_t begin = clock();
		
		cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, ROWS, COLUMNS,
					COLUMNS, 1.0, A, ROWS, B, ROWS, 0.0, C, ROWS);
		
		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		double flops = FLOP / time_spent;

		printf("Iteration: %d time: %f flops: %f\n", i, time_spent, flops);
		
		float *aux=C;
		C=A;
		A=aux;
	}

	return 0;
}
