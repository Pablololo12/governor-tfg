#include "pid.h"
#include <stdio.h>


// Working frequencies
static int NUM_FREQ = 0;
static int posible_freq[30];

// Constants for the formula
static float E = 0;
static float F = 0;
static float A = 918+10000/25000; // K = (fmax - f0) / error_maximo
static float B = 0;
static float C = 0;

static FILE *fp_log;

/*
 * Choose the correct frequency
 */
int which_freq(float freq)
{
	int freq_int = (int) freq;
	int i;
	if(freq_int < posible_freq[0]) {
		return posible_freq[0];
	}

	for (i=1; i<NUM_FREQ; i++) {
		if (freq_int < posible_freq[i]) {
			int mean = (posible_freq[i] + posible_freq[i-1]) >> 1;
			
			if (freq_int < mean) {
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
int update_temp(float error)
{
	// Global variables
	static float u1 = 0;
	static float u2 = 0;
	static float e1 = 0;
	static float e2 = 0;

	float u = -E * u1 - F * u2 + A * error + B * e1 + C * e2;

	u2 = u1;
	u1 = u;
	e2 = e1;
	e1 = error;

	fprintf(fp_log, "%f %f\n", error, u);

	return which_freq(u);
}

/*
 * Initialize variables for PID and frequencies
 */
int initialize_pid()
{
	FILE *fp_const;
	FILE *fp_freq;

	fp_const = fopen(CONST_FILE, "r");
	fp_log = fopen(LOG_FILE, "w");

	if(fp_const==NULL) {
		fprintf(stderr, "Error openning CONST_FILE\n");
		return -1;
	}

	fscanf(fp_const, "%f %f %f %f %f", &E, &F, &A, &B, &C);

	fclose(fp_const);

	fp_freq = fopen(FREQ_FILE, "r");

	if(fp_freq==NULL) {
		fprintf(stderr, "Error openning CONST_FILE\n");
		return -1;
	}

	while(!feof(fp_freq)) {
		fscanf(fp_freq, "%d", &posible_freq[NUM_FREQ++]);
	}
	NUM_FREQ--;

	fclose(fp_const);

	return 1;
}
