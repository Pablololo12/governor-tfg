#ifndef _PID_LIB_
#define _PID_LIB_

#define CONST_FILE "/home/linaro/constants.txt"
#define FREQ_FILE "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies"

// Choose the correct frequency
unsigned int which_freq(unsigned int);

// Update the temperature and return the choosen frequency 
unsigned int update_temp(int);

// initialize the constants for the formula
int initialize_pid();

#endif