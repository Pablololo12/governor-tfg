import sys
import re
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 3:
	print('Error in arguments')
	sys.exit(0)

# First we draw the temp plot
file_temp = open(sys.argv[1], 'r')

# Reading of all the lines
temp_lines = file_temp.readlines()

# Array to save values
temp_values = []
freq_values = []

# Extract numbers from the file
#for line in temp_lines:
#	temp_values.append(float(re.findall(r'\d+[\.]?\d*', line)[0])/1000)
for line in temp_lines:
	aux = re.findall(r'\d+[\.]?\d*', line)

i=0
while i < len(aux)-1:
	temp_values.append(float(aux[i])/1000)
	freq_values.append(int(aux[i+1])/1000)
	i = i + 2

size_temp = len(temp_values)
# Array yo numpy array
temp_values = np.array(temp_values)
freq_values = np.array(freq_values)
# Get all the x values in order to plot
x = np.arange(0, size_temp, 1)

# Functions required to plot
def f_temp(x):
	return temp_values[x]
def f_freq(x):
	return freq_values[x]

# Show Temperature and frequency in the same plot
fig, ax1 = plt.subplots()
plt.title('Temperature-Frequency')
ax1.set_xlabel('Time')
ax1.plot(x,f_temp(x),'b')
ax1.set_ylabel('Temp (C)', color='b')

ax2 = ax1.twinx()
ax2.plot(x,f_freq(x),'g')
ax2.set_ylabel('Freq (MHz)', color='g')

fig.tight_layout()

# Show only the temperature
plt.figure(2)
plt.title('Temperature')
plt.plot(x,f_temp(x),'r')
plt.xlabel('Time')
plt.ylabel('Temp (C)')

# Show only the frequency
plt.figure(3)
plt.title('Frequency')
plt.plot(x,f_freq(x),'g')
plt.xlabel('Time')
plt.ylabel('Freq (MHz)')

# Now plot the results
file_stats = open(sys.argv[2], 'r')

# Reading of all the lines
result_lines = file_stats.readlines()

# Array to save values
iter_values = []
time_values = []
flops_values = []

# Extract numbers from the file
for line in result_lines:
	aux = re.findall(r'\d+[\.]?\d*', line)
	iter_values.append(int(aux[0]))
	time_values.append(float(aux[1]))
	flops_values.append(float(aux[2]))

iter_values = np.array(iter_values)
time_values = np.array(time_values)
flops_values = np.array(flops_values)

y_pos = np.arange(len(iter_values))

plt.figure(4)
plt.title('Benchmarck results (time)')
plt.bar(y_pos, time_values, align='center', alpha=0.5)
plt.xticks(y_pos,iter_values)
plt.ylabel('Time spent (seconds)')
plt.xlabel('Iteration')

plt.figure(5)
plt.title('Benchmark results (Mflops)')
plt.bar(y_pos, flops_values, align='center', alpha=0.5)
plt.xticks(y_pos,iter_values)
plt.ylabel('Mflops')
plt.xlabel('Iteration')

plt.show()