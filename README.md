# Trabajo fin de grado / Final degree proyect
# Pablo Hernández Almudi
pabloheralm@gmail.com

Un governor para linux basado en un controlador PID para mantener una temperatura constante.

A linux governor based on a PID controler to keep a constant temperature.

## Ficheros

* benchmark/ - Carpeta con el programa y script usados para calentar el procesador
    - main.c programa de benchmark
    - Makefile usado para compilar el benchmark
    - plot_results.py muestra resultados de funcionamiento del PID de usuario
    - show_results.py muestra resultados y funcionamiento del governor
    - heater.sh script para probar PID de usuario
    - test_governor.sh script para probar el governor
    - tempLog.sh script para guardar información sobre frecuencia y temperatura
    - test_gov/ resultados de test con el governor
    - tests/ y tests2/ resultads de pruebas con PID usuario
* results/ - Carpeta con los resultados del calentamiento
* governor/ - carpeta con fuentes del governor
  * PID_governor.c - fichero principal del governor
* Matlab_files/ - Carpeta con los programas de Matlab de analisis
* PID_controler/ - Carpeta con la libreria del PID
  * graph_lib/ - libreria que muestra por terminal con ncurses la grafica en tiempo real de la temperatura