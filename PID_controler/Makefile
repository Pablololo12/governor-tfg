# In case of error is needed to export LD_LIBRARY_PATH=/home/linaro/gcc-4.8/lib/

CC=gcc

WARNINGS=-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
         -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
         -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
         -Wuninitialized -Wconversion -Wstrict-prototypes

CFLAGS=-std=c11 -O3 $(WARNINGS)

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
