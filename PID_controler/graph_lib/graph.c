#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include "graph.h"

static WINDOW * mainwin;

/*
 * Draw a graph given an array of values
 */
int print_graph(int * values, unsigned int size)
{
	int max_value=0;
	int i,d;
	int x,y,aux;
	int proportion_y = 1, proportion_x = 1;

	erase();

	// get size of windows
	getmaxyx(mainwin, y, x);
	
	// Get max value of the array
	for(i=0; i<size; i++) {
		if(values[i]>max_value)
			max_value=values[i];
	}

	// Check the proportion_y of the x
	if(max_value>y-3) {
		for(i=2; ;i++) {
			if((max_value/i)<y-3){
				proportion_y = i;
				break;
			}
		}
	}

	// Check proportion_x of the x-axis
	if(size>x-5) {
		for(i=2; ;i++) {
			if((size/i)<x-5){
				proportion_x = i;
				break;
			}
		}
	}

	// Draw vertical line
	color_set(2, NULL);
	d = 0;
	for(i=0; i<y-2; i++) {
		mvaddstr(i, 3, "|");
		mvprintw(y-i-2, 0, "%d", d);
		d += proportion_y;
	}
	mvprintw(y-i-2, 0, "%d", d);

	// Draw horizontal lines
	for(i=3; i<x; i++) {
		mvaddstr(y-2, i,"-");
	}

	// Set color to red
	color_set(1, NULL);
	
	// Loop to print the graph
	for(i=0, d=0; d<size; i++, d+=proportion_x) {
		aux = values[d]/proportion_y;
		mvaddstr(y-aux-2, i + 4, "~");
	}

	refresh();

	return 0;
}

int initialize()
{
	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses.\n");
		exit(EXIT_FAILURE);
	}

	start_color();

	if ( has_colors() && COLOR_PAIRS >= 13 ) {

		init_pair(1,  COLOR_RED,     COLOR_BLACK);
		init_pair(2,  COLOR_WHITE,   COLOR_BLACK);
	}

	refresh();

	return 1;
}

int destroy()
{
	delwin(mainwin);
	endwin();
	refresh();

	return 0;
}

int main()
{
	int matrix[1024];
	int siz = 0;
	initialize();
	int i;
	for (i=0; i<1024; i++) {
		matrix[i] = i;
		siz++;
		print_graph(matrix, siz);
		sleep(1);
	}
	destroy();
	return 0;
}