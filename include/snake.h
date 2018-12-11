#include <stdint.h>
#include <stdbool.h>

#ifndef SNAKE_H_

#define SNAKE_H_

#define BACKGROUND 1
#define SNAKE 2
#define FOOD 3
#define TEXT 4

const char *PIECE = "  ";

typedef struct snake snake;
typedef struct snake_segment snake_segment;

typedef enum _DIRECTION DIRECTION;

void init_board(snake *s);
void init_snake(snake *s);
void add_food(snake *s);
void update_snake(snake *s, DIRECTION direction);
struct snake_segment *new_head(int16_t x, int16_t y, snake_segment *next);
void draw_end_game(void);

struct snake_segment
{
    int16_t x;
    int16_t y;

    struct snake_segment *next;
};

struct snake
{
    uint16_t rows;
    uint16_t cols;

    uint16_t food_x;
    uint16_t food_y;

    struct snake_segment *head;
    struct snake_segment *tail;
};

enum _DIRECTION
{
    N,
    E,
    S,
    W
};

#endif
