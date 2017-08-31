import sys
import re
import os
import numpy as np
import matplotlib.pyplot as plt

font = {'family' : 'Bitstream Vera Sans',
        'weight' : 'bold',
        'size'   : 18}

plt.rc('font', **font)
plt.rc('lines', linewidth=4)

# Array to save values
iter_values_1 = [0] * 10
flops_values_max_1 = [0.0] * 10
flops_values_min_1 = [20.0] * 10
flops_values_media_1 = [0.0] * 10
number_results_1 = 0

#file_stats = open(sys.argv[2], 'r')
for file in os.listdir(sys.argv[1]):
	if file.endswith(".txt"):
		print(os.path.join(sys.argv[1], file))
		file_stats = open(os.path.join(sys.argv[1], file),'r');
		# Reading of all the lines
		result_lines = file_stats.readlines()
		# Extract numbers from the file
		i=0
		number_results_1 = number_results_1 + 1
		for line in result_lines:
			aux = re.findall(r'\d+[\.]?\d*', line)
			iter_values_1[i] = int(aux[0])
			tt = float(aux[2])
			time = float(aux[1])
			tt = 1099444518912/time;
			tt = tt / 1000000000
			flops_values_media_1[i] = flops_values_media_1[i] + tt
			if tt > flops_values_max_1[i]:
				flops_values_max_1[i] = tt
			if tt < flops_values_min_1[i]:
				flops_values_min_1[i] = tt
			i = i + 1

for d in range(0,len(flops_values_media_1)):
	flops_values_media_1[d] = flops_values_media_1[d] / number_results_1

flops_values_max_2 = [0.0] * 10
flops_values_min_2 = [20.0] * 10
flops_values_media_2 = [0.0] * 10
number_results_2 = 0

#file_stats = open(sys.argv[2], 'r')
for file in os.listdir(sys.argv[2]):
	if file.endswith(".txt"):
		print(os.path.join(sys.argv[2], file))
		file_stats = open(os.path.join(sys.argv[2], file),'r');
		# Reading of all the lines
		result_lines = file_stats.readlines()
		# Extract numbers from the file
		i=0
		number_results_2 = number_results_2 + 1
		for line in result_lines:
			aux = re.findall(r'\d+[\.]?\d*', line)
			tt = float(aux[2])
			time = float(aux[1])
			tt = 1099444518912/time;
			tt = tt / 1000000000
			flops_values_media_2[i] = flops_values_media_2[i] + tt
			if tt > flops_values_max_2[i]:
				flops_values_max_2[i] = tt
			if tt < flops_values_min_2[i]:
				flops_values_min_2[i] = tt
			i = i + 1

for d in range(0,len(flops_values_media_2)):
	flops_values_media_2[d] = flops_values_media_2[d] / number_results_2

iter_values_1 = np.array(iter_values_1)
flops_values_max_1 = np.array(flops_values_max_1)
flops_values_min_1 = np.array(flops_values_min_1)
flops_values_media_1 = np.array(flops_values_media_1)
flops_values_max_2 = np.array(flops_values_max_2)
flops_values_min_2 = np.array(flops_values_min_2)
flops_values_media_2 = np.array(flops_values_media_2)

y_pos = np.arange(len(iter_values_1))

plt.figure(1)
#plt.title('Resultados Benchmark (Mflops)')
w = 0.20
w2 = 0.4
plt.bar(y_pos-w, flops_values_max_1, width=w2, align='center', color='c', alpha=0.5, label="PID max")
plt.bar(y_pos-w, flops_values_media_1, width=w2, align='center', color='g', alpha=0.5, label="PID media")
plt.bar(y_pos-w, flops_values_min_1, width=w2, align='center', color='b', alpha=0.5, label="PID min")

plt.bar(y_pos+w, flops_values_max_2, width=w2, align='center', color='m', alpha=0.5, label="Defecto max")
plt.bar(y_pos+w, flops_values_media_2, width=w2, align='center', color='yellow', alpha=0.5, label="Defecto media")
plt.bar(y_pos+w, flops_values_min_2, width=w2, align='center', color='red', alpha=0.5, label="Defecto min")
plt.legend(prop={'size': 14}, loc=3)
plt.xticks(y_pos,iter_values_1)
plt.ylabel('Mflops')
plt.xlabel('Iteracion')
plt.xlim([-1,len(iter_values_1)])
plt.ylim([0,7])
plt.tight_layout()
plt.savefig(sys.argv[3]+".pdf", format="pdf")
plt.show()