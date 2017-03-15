#include "pid.h"
#include <stdio.h>

// Global variables
static int previous_temp = 0;
static int error_minus1 = 0;
static int error_minus2 = 0;

// Constants for the formula
static int E = 0;
static int A = 918+10000/25000;
static int B = 0;
static int C = 0;

// Working frequencies
static int NUM_FREQ = 0;
static unsigned int posible_freq[30];

/*
 * Choose the correct frequency
 */
unsigned int which_freq(unsigned int freq)
{
	int i;
	if(freq < posible_freq[0]) {
		return posible_freq[0];
	}

	for (i=1; i<NUM_FREQ; i++) {
		if (freq < posible_freq[i]) {
			unsigned int mean = (posible_freq[i] + posible_freq[i-1]) >> 1;
			
			if (freq < mean) {
				return posible_freq[i-1];
			} else {
				return posible_freq[i];
			}
		}
	}

	return posible_freq[NUM_FREQ - 1];
}

/*
 * Return the temperature
 */
unsigned int update_temp(int error)
{
	int acum = E * previous_temp + A * error + B * error_minus1 + C * error_minus2;
	previous_temp = acum;
	error_minus2 = error_minus1;
	error_minus1 = error;

	return which_freq(acum);
}

/*
 * Initialize variables for PID and frequencies
 */
int initialize()
{
	FILE *fp_const;
	FILE *fp_freq;

	fp_const = fopen(CONST_FILE, "r");

	if(fp_const==NULL) {
		fprintf(stderr, "Error openning CONST_FILE\n");
		return -1;
	}

	fscanf(fp_const, "%d %d %d %d", &E, &A, &B, &C);

	fclose(fp_const);

	fp_freq = fopen(FREQ_FILE, "r");

	if(fp_freq==NULL) {
		fprintf(stderr, "Error openning CONST_FILE\n");
		return -1;
	}

	while(!feof(fp_freq)) {
		fscanf(fp_freq, "%d", &posible_freq[NUM_FREQ++]);
	}

	fclose(fp_const);

	return 1;
}

int main()
{
	initialize();
	return 0;
}