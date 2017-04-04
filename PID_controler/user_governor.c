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
#include <unistd.h>

// Constants
enum {
        DESIRED_TEMP = 70000, // The desired temperature TODO: File config
        ELEMENST_TO_ADD = 200}; // Constant for the array of values

// TODO: Find by itself the correct thermal zone
#define FILE_TEMP "/sys/class/thermal/thermal_zone0/temp"

// Directory of cpus
#define PATH_TO_CPU "/sys/devices/system/cpu"

// Files to get temperature info and set frequencies
static int temp;
static int num_cpus = 0;
static int *cpus_fd;

/*
 * Function to open the temperature file and the freq files
 */
static int open_files(void)
{
	// open the temperature file
	temp = open(FILE_TEMP, O_RDONLY);
	if (temp == -1) {
		fprintf(stderr, "Error openning temperature file\n");
		return -1;
	}
	
	// Get the number of cpus that are currently online
	num_cpus = (int) sysconfig(_SC_NPROCESSORS_ONLN);
	cpus_fd = (int *) malloc(num_cpus * sizeof(int));
	// Array to keep the path of cpus
	char path[255];
	int i;

	for (i=0; i<num_cpus; i++) {
		// path and snprintf thanks to 
		// https://github.com/tud-zih-energy/libadapt/blob/master/knobs/fastcpufreq.c

		// Get the path of the current cpu starting with 0
		snprintf(path, sizeof(path),
		         PATH_TO_CPU "/cpu%u/cpufreq/scaling_setspeed",
		         i);
		cpus_fd[i] = open(path, O_WRONLY);
		
		if (cpus_fd[i] == -1) {
			fprintf(stderr, "Error openning cpu%d\n",i);
			return -1;
		}
	}

	return 1;
}

int main(void)
{
	int size = ELEMENST_TO_ADD; // Size of the array of the latest temperatures
	// Array to keep an historic of the temperatures
	int *temp_historic = malloc((unsigned long)(size*(int)(sizeof(int))));
	int number_elements=0; // Pointer of the historic array
	int aux = 0; // Auxiliar variable to keep temperature
	int new_freq = 0; // Auxiliar variable to save new frequency
	char freq_str[8]; // Char array used to write frequency
	char temp_buff[8]; // Char array used to read the current temperature
	int i; // Auxiliary variable

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
			size += ELEMENST_TO_ADD;
			temp_historic = realloc(temp_historic,
			                        (unsigned long)(size * (int)(sizeof(int))));
			if (temp_historic == NULL) {
				return 1;
			}
		}

		// Print the new graph
		print_graph(temp_historic, number_elements);

		// Update the PID
		new_freq = update_temp(DESIRED_TEMP-aux);

		// Set the new freq
		sprintf(freq_str,"%d", new_freq);
		// Set the frequency to each cpu
		for (i=0; i<num_cpus; i++) {
			pwrite(cpus_fd[i], freq_str, strlen(freq_str),0);
		}

		sleep(3);
	}

	return 0;
}
