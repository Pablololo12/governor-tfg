#include "pid.h"

#define NUM_FREQ 6

// Constants for the formula
const int E = 1;
const int A = 1;
const int B = 1;
const int C = 1;

// Global variables
int previous_temp = 0;
int error_minus1 = 0;
int error_minus2 = 0;

// Working frequencies
unsigned int posible_freq[NUM_FREQ] = {384000, 486000, 594000, 702000, 810000, 918000};

/*
 * Choose the correct frequency
 */
unsigned int which_freq(unsigned int freq)
{
	int i;
	if(freq < posible_freq[0])
	{
		return posible_freq[0];
	}

	for(i=1; i<NUM_FREQ; i++)
	{
		if(freq < posible_freq[i])
		{
			unsigned int mean = (posible_freq[i] + posible_freq[i-1]) >> 1;
			
			if(freq < mean)
			{
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
int update_temp(int error)
{
	int acum = E * previous_temp + A * error + B * error_minus1 + C * error_minus2;
	previous_temp = acum;
	error_minus2 = error_minus1;
	error_minus1 = error;

	return acum;
}

int main(int argc, char ** argv)
{
	return 0;
}