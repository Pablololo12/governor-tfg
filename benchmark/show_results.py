import sys
import re
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 3:
	print('Error in arguments')
	sys.exit(0)


font = {'family' : 'normal',
        'weight' : 'bold',
        'size'   : 18}

plt.rc('font', **font)
plt.rc('lines', linewidth=4)


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
aux = []
for line in temp_lines:
	 aux.append(re.findall(r'\d+[\.]?\d*', line))

i=0
while i < len(aux[0])-1:
	temp_values.append(float(aux[0][i])/1000)
	freq_values.append(int(aux[0][i+1])/1000)
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

plt.figure(1)
plt.title('Freq')
plt.xlabel('Tiempo')
plt.plot(x,f_freq(x),'b')
plt.ylabel('Freq Real (MHz)', color='b')

plt.figure(2)
plt.title('Temp')
plt.xlabel('Tiempo')
plt.plot(x,f_temp(x),'b')
plt.ylabel('Temp', color='b')


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
plt.title('Resultados Benchmark (Mflops)')
plt.bar(y_pos, flops_values, align='center', alpha=0.5)
plt.xticks(y_pos,iter_values)
plt.ylabel('Mflops')
plt.xlabel('Iteracion')
plt.xlim([-1,len(iter_values)])
plt.tight_layout()

plt.show()