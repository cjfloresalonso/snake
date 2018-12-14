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

// model/controller functions
void mainloop(game *g);
void init_prog(void);
game * init_game(void);
void add_food(game *g);
void update_snake(game *g, DIRECTION direction);
void update_food(game *g);
bool is_collided(game *g);
struct snake_segment *new_segment(int16_t x, int16_t y, snake_segment *next);

// view functions
void init_board(game *g);
void draw_snake(snake_segment *head, int16_t count, int32_t colour);
void draw_food(game *g);
void has_collided(game *g);
WINDOW *new_pause_window(game *g);
void kill_pause_window(WINDOW *pwindow);
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
    N = KEY_UP,
    E = KEY_RIGHT,
    S = KEY_DOWN,
    W = KEY_LEFT
};

#endif
