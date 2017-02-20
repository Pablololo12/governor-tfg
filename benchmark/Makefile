# In case of error is needed to export LD_LIBRARY_PATH=/home/linaro/gcc-4.8/lib/

CC=g++

CFLAGS=-std=c++11 -O3 -Wall

LFLAGS=-I/home/linaro/gcc-4.8/include -L/home/linaro/gcc-4.8/lib

LIBS=-lm -lQSML-sequential

all:
	$(CC) $(LFLAGS) $(CFLAGS) -o benchmark main.c