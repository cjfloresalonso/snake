#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "snake.h"

int main(int argc, char **argv)
{
	struct board *bd = init_board(49,150);

	draw_board(bd);

	return 0;
}

void draw_board(struct board *b)
{
	fputs("\033[2J", stdout);

	fputs(b->matrix, stdout);

	// fputs("\033[H", stdout);
	fputs("\n", stdout);

}
void draw_snake(struct board *b, struct snake *s)
{
    return;
}

struct board * init_board(int height, int width)
{
	// get memory
	struct board *bd = malloc(sizeof(struct board));
	
    bd->matrix = malloc(sizeof(char) * height * (width+1));
	char *row = bd->matrix;
	
	// set dimensions
	bd->width = width;
    bd->height = height;
	
	// populate matrix
	empty_header(row, width);
	row += width+2;

	while (row < bd->matrix + (height - 1) * (width + 1))
	{
		empty_body(row, width);
		row += width+2;
	}

	empty_footer(row, width);
	row += width+2;

	// fin
	return bd;
}

void empty_header(char *line, int width)
{
    if (!line)
	{
        return;
	}

	char *c = line;
	*c = '0';
	while (c < line + width - 1)
		*++c = '-';
	
	*++c = '0';
	*++c = '\n';
} 

/*
 *	append an empty body string to so string.
 * 
 * 	the string must have enough memory before the end.
 * 	
 * 	Arguments:
 * 		line: string to append to (at the point to append)
 * 		width: width of the body.
**/
void empty_body(char *line, int width)
{
 	if (!line)
	{
		return;
	}

	char *c = line;
 	*c = '|';
	while (c < line + width - 1)
    	*++c = ' ';
 	
	*++c = '|';
	*++c = '\n';
}

/*
 * 	append an emtpy footer string to so string.
 *
 *	the string must have enough memory before the end.
 *
 * 	Arguments:
 *      line: string to append to (at the point to append)
 *      width: width of the body.
**/
void empty_footer(char *line, int width)
{	
    if (!line)
    {
	    return;
	}

	empty_header(line, width);
	*(line + width + 1) = '\0';
}
