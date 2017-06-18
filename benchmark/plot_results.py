import sys
import re
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 7:
	print('Error in arguments')
	sys.exit(0)

# temperaturas
# error
# benchmark
# temp obj
# ruta guardado
# extension del nombre

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
while i < len(aux)-1:
	temp_values.append(float(aux[i][0])/1000)
	freq_values.append(int(aux[i][1])/1000)
	i = i + 1

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


#Error file
file_error = open(sys.argv[2], 'r')

# Read all errors and u
error_lines = file_error.readlines()

error_values = []
theoric_u = []

aux = []
for line in error_lines:
	aux.append(re.findall(r'\d+[\.]?\d*', line))

i=0
while i < len(aux)-1:
	error_values.append(float(aux[i][0]))
	theoric_u.append(float(aux[i][1])/1000.0)
	i = i + 1

size_error = len(error_values)
# Array yo numpy array
error_values = np.array(error_values)
theoric_u = np.array(theoric_u)
# Get all the x values in order to plot
y = np.arange(0, size_error, 1)

if size_error < size_temp:
	x=y

# Functions required to plot
def f_error(x):
	return error_values[x]
def f_thu(x):
	return theoric_u[x]

# Show Temperature and frequency in the same plot
#plt.tight_layout()

plt.figure(1)
plt.title('Freq Teorica y Discreta')
plt.xlabel('Tiempo')
plt.plot(x,f_thu(x),'b')
plt.ylabel('Freq Real (MHz)', color='b')
plt.plot(x,f_freq(x),'g')
plt.xlim([0,size_temp])
plt.tight_layout()

plt.savefig(sys.argv[5]+"/freq_"+sys.argv[6]+".pdf", format="pdf")

# Show only the temperature
plt.figure(2)
plt.title('Temperatura')
plt.plot([0, size_temp], [sys.argv[4], sys.argv[4]], '--')
plt.plot(x,f_temp(x),'r')
plt.xlabel('Tiempo')
plt.ylabel('Temp (C)')
plt.xlim([0,size_temp])
plt.tight_layout()
plt.savefig(sys.argv[5]+"/temp_"+sys.argv[6]+".pdf", format="pdf")

# Show only the error
plt.figure(3)
plt.title('Error')
plt.plot(x,f_error(x),'r')
plt.xlabel('Tiempo')
plt.ylabel('Temp (C)')
plt.xlim([0,size_error])
plt.tight_layout()
plt.savefig(sys.argv[5]+"/error_"+sys.argv[6]+".pdf", format="pdf")
'''
# Show Temperature and frequency in the same plot
fig2, ax3= plt.subplots()
plt.title('Error-Frecuencia continua')
ax3.set_xlabel('Time')
ax3.plot(x,f_error(x),'b')
ax3.set_ylabel('Diff Temp (C)', color='b')

ax4 = ax3.twinx()
ax4.plot(x,f_thu(x),'g')
ax4.set_ylabel('Freq (MHz)', color='g')

fig2.tight_layout()
'''

# Now plot the results
file_stats = open(sys.argv[3], 'r')

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
'''
plt.figure(4)
plt.title('Benchmarck results (time)')
plt.bar(y_pos, time_values, align='center', alpha=0.5)
plt.xticks(y_pos,iter_values)
plt.ylabel('Time spent (seconds)')
plt.xlabel('Iteration')
'''
plt.figure(4)
plt.title('Resultados Benchmark (Mflops)')
plt.bar(y_pos, flops_values, align='center', alpha=0.5)
plt.xticks(y_pos,iter_values)
plt.ylabel('Mflops')
plt.xlabel('Iteracion')
plt.xlim([-1,len(iter_values)])
plt.tight_layout()
plt.savefig(sys.argv[5]+"/bench_"+sys.argv[6]+".pdf", format="pdf")

#plt.show()