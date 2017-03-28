// In order to use pread and pwrite
#define _XOPEN_SOURCE 500
#include <unistd.h>
///////////////////
#include "pid.h"
#include "graph_lib/graph.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

// TODO: Find by itself the correct thermal zone
#define FILE_TEMP "/sys/class/thermal/thermal_zone0/temp"

// TODO: Discover automatically the number of cpus
#define CPU0_FREQ "/sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed"
#define CPU1_FREQ "/sys/devices/system/cpu/cpu1/cpufreq/scaling_setspeed"
#define CPU2_FREQ "/sys/devices/system/cpu/cpu2/cpufreq/scaling_setspeed"
#define CPU3_FREQ "/sys/devices/system/cpu/cpu3/cpufreq/scaling_setspeed"

// Files to get temperature info and set frequencies
static int temp;
static int cpu_0;
static int cpu_1;
static int cpu_2;
static int cpu_3;

/*
 * Function to open the temperature file and the freq files
 */
static int open_files(void)
{
	temp = open(FILE_TEMP, O_RDONLY);
	if (temp == -1) {
		fprintf(stderr, "Error openning temperature file\n");
		return -1;
	}
	
	cpu_0 = open(CPU0_FREQ, O_WRONLY);
	if (cpu_0 == -1) {
		fprintf(stderr, "Error openning cpu0\n");
		return -1;
	}

	cpu_1 = open(CPU1_FREQ, O_WRONLY);
	if (cpu_1 == -1) {
		fprintf(stderr, "Error openning cpu1\n");
		return -1;
	}

	cpu_2 = open(CPU2_FREQ, O_WRONLY);
	if (cpu_2 == -1) {
		fprintf(stderr, "Error openning cpu2\n");
		return -1;
	}

	cpu_3= open(CPU3_FREQ, O_WRONLY);
	if (cpu_3 == -1) {
		fprintf(stderr, "Error openning cpu3\n");
		return -1;
	}
	return 1;
}

int main(void)
{
	int size=200;
	int *temp_historic = malloc((unsigned long)(size*(int)(sizeof(int))));
	int number_elements=0;
	int aux = 0;
	int new_freq = 0;
	char freq_str[8];
	char temp_buff[8];

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
		pread(temp,temp_buff,8, 0);
		aux = (int) strtol(temp_buff,NULL,10);
		
		// Add the temperature to the historic of temperatures
		temp_historic[number_elements++] = aux/1000;
		// If the array is full more space is added
		if(number_elements == size) {
			size += 200;
			temp_historic = realloc(temp_historic, (unsigned long)(size * (int)(sizeof(int))));
		}

		// Print the new graph
		print_graph(temp_historic, number_elements);

		// Update the PID
		new_freq = update_temp(85000-aux);

		// Set the new freq
		sprintf(freq_str,"%d", new_freq);
		pwrite(cpu_0,freq_str,strlen(freq_str),0);
		pwrite(cpu_1,freq_str,strlen(freq_str),0);
		pwrite(cpu_2,freq_str,strlen(freq_str),0);
		pwrite(cpu_3,freq_str,strlen(freq_str),0);

		sleep(3);
	}

	return 0;
}
