import sys
import re
import os
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 3:
	print('Error in arguments')
	sys.exit(0)


font = {'family' : 'Bitstream Vera Sans',
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


# Now plot the mflops

# Array to save values
iter_values = [0] * 10
time_values = [0] * 10
flops_values_max = [0.0] * 10

flops_values_min = [20.0] * 10

#file_stats = open(sys.argv[2], 'r')
for file in os.listdir(sys.argv[2]):
	if file.endswith(".txt"):
		print(os.path.join(sys.argv[2], file))
		file_stats = open(os.path.join(sys.argv[2], file),'r');
		# Reading of all the lines
		result_lines = file_stats.readlines()
		# Extract numbers from the file
		i=0
		for line in result_lines:
			aux = re.findall(r'\d+[\.]?\d*', line)
			iter_values[i] = int(aux[0])
			time_values[i] = float(aux[1])
			tt = float(aux[2])
			if tt > flops_values_max[i]:
				flops_values_max[i] = tt
			if tt < flops_values_min[i]:
				flops_values_min[i] = tt
			i = i + 1

iter_values = np.array(iter_values)
time_values = np.array(time_values)
flops_values_max = np.array(flops_values_max)
flops_values_min = np.array(flops_values_min)

y_pos = np.arange(len(iter_values))

plt.figure(4)
plt.title('Resultados Benchmark (Mflops)')
plt.bar(y_pos, flops_values_max, align='center', color='blue', alpha=0.5)
plt.bar(y_pos, flops_values_min, align='center', color='red', alpha=0.5)
plt.xticks(y_pos,iter_values)
plt.ylabel('Mflops')
plt.xlabel('Iteracion')
plt.xlim([-1,len(iter_values)])

plt.show()