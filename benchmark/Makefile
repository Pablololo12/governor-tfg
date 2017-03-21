# In case of error is needed to export LD_LIBRARY_PATH=/home/linaro/gcc-4.8/lib/

CC=g++

WARNINGS=-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
         -Wwrite-strings -Wmissing-declarations \
         -Wredundant-decls  -Winline -Wno-long-long \
         -Wuninitialized -Wconversion

CFLAGS=-std=c++11 -O3 $(WARNINGS)

LFLAGS=-I/home/linaro/gcc-4.8/include -L/home/linaro/gcc-4.8/lib

LIBS=-lm -lQSML

all:
	$(CC) $(LFLAGS) $(CFLAGS) -o benchmark main.c $(LIBS)
