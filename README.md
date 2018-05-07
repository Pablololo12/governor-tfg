# Trabajo fin de grado / Final degree proyect
# Pablo Hernández Almudi
pabloheralm@gmail.com

## Description

A Linux governor based on a PID controller which keeps a constant temperature
under a constant load.

## Files

* benchmark/ - Folder with the benchmark and scripts
    - main.c benchmark main file
    - Makefile for the benchmark
    - heater.sh script for automatic test with the userspace governor
    - test_governor.sh script for automatic test of PID governor
    - plot_results.py, show_results.py and compare.py Python scripts to plot graphics
    - tempLog.sh script to log temperature and frequency
* Matlab_files/ - Folder with diferent Matlab scripts
* PID_controler/ - Folder with the PID controller on userspace
    - user_governor.c main file of the controller
    - Makefile for the controller
    - pid.c and pid.h the controller as a library
    - graph_lib/ a library to show graphics on the terminal
* governor/ - Folder with the governor source
    - PID_governor.c the source code
    - kernel_compile.md instructions to add the governor to the kernel
* results/ - Folder with results

---

## Descripción

Un governor para linux basado en un controlador PID que mantiene una temperatura
constante bajo una carga constante.

## Ficheros

* benchmark/ - Carpeta con el programa y script usados para calentar el procesador
    - main.c programa de benchmark
    - Makefile usado para compilar el benchmark
    - plot_results.py muestra resultados de funcionamiento del PID de usuario
    - show_results.py muestra resultados y funcionamiento del governor
    - heater.sh script para probar PID de usuario
    - test_governor.sh script para probar el governor
    - tempLog.sh script para guardar información sobre frecuencia y temperatura
* results/ - Carpeta con los resultados del calentamiento
* governor/ - carpeta con fuentes del governor
  * PID_governor.c - fichero principal del governor
* Matlab_files/ - Carpeta con los programas de Matlab de analisis
* PID_controler/ - Carpeta con la libreria del PID
  * graph_lib/ - libreria que muestra por terminal con ncurses la grafica en tiempo real de la temperatura