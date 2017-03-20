#include "pid.h"
#include "graph_lib/graph.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// TODO: Find by itself the correct thermal zone
#define FILE_TEMP "/sys/class/thermal/thermal_zone0/temp"

// TODO: Discover automatically the number of cpus
#define CPU0_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
#define CPU1_FREQ "/sys/devices/system/cpu/cpu1/cpufreq/scaling_setspeed"
#define CPU2_FREQ "/sys/devices/system/cpu/cpu2/cpufreq/scaling_setspeed"
#define CPU3_FREQ "/sys/devices/system/cpu/cpu3/cpufreq/scaling_setspeed"

// Files to get temperature info and set frequencies
static FILE *temp;
static FILE *cpu_0;
static FILE *cpu_1;
static FILE *cpu_2;
static FILE *cpu_3;

/*
 * Function to open the temperature file and the freq files
 */
int open_files()
{
	temp = fopen(FILE_TEMP, "r");
	if (temp == NULL) {
		fprintf(stderr, "Error openning temperature file\n");
		return -1;
	}
	
	cpu_0 = fopen(CPU0_FREQ, "w");
	if (cpu_0 == NULL) {
		fprintf(stderr, "Error openning cpu0\n");
		return -1;
	}

	cpu_1 = fopen(CPU1_FREQ, "w");
	if (cpu_1 == NULL) {
		fprintf(stderr, "Error openning cpu1\n");
		return -1;
	}

	cpu_2 = fopen(CPU2_FREQ, "w");
	if (cpu_2 == NULL) {
		fprintf(stderr, "Error openning cpu2\n");
		return -1;
	}

	cpu_3= fopen(CPU3_FREQ, "w");
	if (cpu_3 == NULL) {
		fprintf(stderr, "Error openning cpu3\n");
		return -1;
	}
	return 1;
}

int main()
{
	unsigned int size=200;
	int *temp_historic = malloc(size*sizeof(int));
	unsigned int number_elements=0;
	int aux = 0;
	unsigned int new_freq = 0;

	// initialize the PID controller
	if (initialize_pid()==-1) {
		return 1;
	}

	// Open control files
	if (open_files()==-1) {
		return 1;
	}

	// Initialize the window
	if (initialize_win()==-1) {
		return 1;
	}

	while (1) {
		// Get the current temperature
		fscanf(temp,"%d", &aux);
		
		// Add the temperature to the historic of temperatures
		temp_historic[number_elements++] = aux;
		// If the array is full more space is added
		if(number_elements == size) {
			size += 200;
			temp_historic = realloc(temp_historic, size * sizeof(int));
		}

		// Print the new graph
		print_graph(temp_historic, number_elements);

		// Update the PID
		new_freq = update_temp(aux);

		// Set the new freq
		fprintf(cpu_0, "%d\n", new_freq);
		fprintf(cpu_1, "%d\n", new_freq);
		fprintf(cpu_2, "%d\n", new_freq);
		fprintf(cpu_3, "%d\n", new_freq);

		sleep(3);
	}

	return 0;
}