import matplotlib.pyplot as plt
import numpy as np
import re
import sys

fichero = open(sys.argv[1],'r')
lineas = fichero.readlines()
num_lineas = len(lineas)

valores_temp = []
valores_freq = []
valores_volt = []

for linea in lineas:
		if(linea[0] == 't'):
			numero = float(re.findall(r'\d+[\.]?\d*', linea)[0])
			valores_temp.append(numero)
		if(linea[0] == 'f'):
			numero = float(re.findall(r'\d+', linea)[1])/1000000
			valores_freq.append(numero)
		if(linea[0] == 'v'):
			numero = float(re.findall(r'\d+[\.]?\d*', linea)[0])
			valores_volt.append(numero)
		valores_temp.append(int(linea))


valores_temp = np.array(valores_temp)
valores_freq = np.array(valores_freq)
valores_volt = np.array(valores_volt)


def f_temp(x):
	return valores_temp[x]

def f_freq(x):
	return valores_freq[x]

def f_volt(x):
	return valores_volt[x]

intervalo = num_lineas
x = np.arange(0,intervalo,1)

plt.subplot(211)
plt.title('Temperatura')
plt.plot(x,f_temp(x))

plt.show()
