# In case of error is needed to export LD_LIBRARY_PATH=/home/linaro/gcc-4.8/lib/

CC=gcc

CFLAGS=-std=c11 -O3 -Wall

LIBS=-lncurses

SRC=pid.h pid.c graph_lib/graph.c graph_lib/graph.h
OBJ=pid.o graph.o

all: pid.o graph.o
	$(CC) $(CFLAGS) $(LIBS) -o user_governor user_governor.c $(OBJ)

pid.o: pid.c
	$(CC) $(CFLAGS) -c pid.c

graph.o: graph_lib/graph.c
	$(CC) $(CFLAGS) $(LIBS) -c graph_lib/graph.c

clean:
	$(RM) $(OBJ) user_governor