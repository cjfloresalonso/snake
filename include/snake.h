#include <stdint.h>
#include <stdbool.h>

#ifndef SNAKE_H_

#define SNAKE_H_

struct snake;
struct board;

struct board * init_board(int height, int width);

void draw_board(struct board *b);
void draw_snake(struct board *b, struct snake *s);

void empty_header(char *line, int width);
void empty_body(char *line, int width);
void empty_footer(char *line, int width);

struct _snake
{
	uint16_t x;
	uint16_t y;
	
	struct _snake *next_segment;
} snake;

struct board
{
	uint16_t height;
	uint16_t width;

	char *matrix;
};

#endif
