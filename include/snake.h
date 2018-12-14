#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef SNAKE_H_

#define SNAKE_H_

#define BACKGROUND 1
#define SNAKE 2
#define FOOD 3
#define TEXT 4

const char *PIECE = "  ";

typedef struct game game;
typedef struct snake_segment snake_segment;

typedef enum _DIRECTION DIRECTION;

void init_prog(void);
void init_board(game *g);
game * init_game(void);
void mainloop(game *g);
void add_food(game *g);
void update_snake(game *g, DIRECTION direction);
void update_food(game *g);
bool is_collided(game *g);
void has_collided(game *g);
struct snake_segment *new_segment(int16_t x, int16_t y, snake_segment *next);
int64_t time_elapsed(game *g);
void pause_game(game *g);
void quit(game *g);
void free_game(game *g);

struct snake_segment
{
    int16_t x;
    int16_t y;

    struct snake_segment *next;
};

struct game
{
    // board dimensions
    uint16_t rows;
    uint16_t cols;

    // food position
    uint16_t food_x;
    uint16_t food_y;

    // number to grow by
    uint8_t grow;

    // current score
    uint32_t score;

    // start time
    int64_t start;

    // snake head and tail
    snake_segment *head;
    snake_segment *tail;
};

enum _DIRECTION
{
    N = 'w',
    E = 'd',
    S = 's',
    W = 'a'
};

#endif
