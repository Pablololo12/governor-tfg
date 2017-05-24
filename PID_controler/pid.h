#ifndef _PID_LIB_
#define _PID_LIB_

#define CONST_FILE "/home/linaro/constants.txt"
#define FREQ_FILE "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies"

// Choose the correct frequency
int which_freq(float freq);

// Update the temperature and return the choosen frequency 
int update_temp(float error);

// initialize the constants for the formula
int initialize_pid(void);

#endif
