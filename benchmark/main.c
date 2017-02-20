#include <qsml.h>
#include <stdio.h>
#include <stdlib.h>

#define ROWS 8192
#define COLUMNS 8192

int main(int argc, char **argv)
{
	uint32_t matrixSize = ROWS*COLUMNS;

	float *A = new float[matrixSize];
	float *B = new float[matrixSize];
	float *C = new float[matrixSize];

	for(uint32_t i=0; i<matrixSize;i++)
	{
		A[i]=(float)rand()/(float)(RAND_MAX/1000);
		B[i]=(float)rand()/(float)(RAND_MAX/1000);
	}

	while(1==1)
	{
		cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, ROWS, COLUMNS, COLUMNS, 1.0, A, ROWS, B, ROWS, 0.0, C, ROWS);
		float *aux=C;
		C=A;
		A=aux;
	}

	return 0;
}
