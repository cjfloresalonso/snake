#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <ncurses.h>

#include "snake.h"

int main(int argc, char **argv)
{
    init_prog();

    // initilise the game
    snake *s = init_snake();
    init_board(s);
    add_food(s);

    // loop for movement
    do
    {
        // get direction to move in
        attron(COLOR_PAIR(TEXT));
        char c = fgetc(stdin);
        attroff(COLOR_PAIR(TEXT));

        // deal with input accordingly
        switch (c)
        {
        case 'd':
            update_snake(s, E);
            break;
        case 'a':
            update_snake(s, W);
            break;
        case 'w':
            update_snake(s, N);
            break;
        case 'q':
        case 'Q':
            return 1;
        case 's':
            update_snake(s, S);
            break;
        default:
            break;
        }

        // flush the any charecters still in the stdin buffer
        fflush(stdin);

    } while (true);

    exit(EXIT_SUCCESS);
}

void init_prog(void)
{
    // initilise the tui
    initscr();

    cbreak();
    noecho();

    curs_set(0);
    start_color();

    // initilise the colour pairs
    init_pair(BACKGROUND, COLOR_WHITE, COLOR_WHITE);
    init_pair(SNAKE, COLOR_BLACK, COLOR_GREEN);
    init_pair(FOOD, COLOR_BLACK, COLOR_RED);
    init_pair(TEXT, COLOR_WHITE, COLOR_BLACK);

    // seed random numbers
    srand(time(NULL));

    // add fucntions to exit
    atexit((void (*)(void))endwin);
}

void init_board(snake *s)
{
    // draw background/board
    // TODO: optimise this
    attron(COLOR_PAIR(BACKGROUND));
    move(0, 0);
    for (int i = 0; i < s->rows; i++)
    {
        for (int j = 0; j < s->cols; j++)
        {
            addstr(PIECE);
        }

        addch('\n');
    }

    attroff(COLOR_PAIR(BACKGROUND));
}

snake *init_snake(void)
{
    // declare snake's memory
    snake *s = malloc(sizeof(snake));

    /// TODO: make random
    // initilise segments of hte snake's body
    snake_segment *sn2 = new_head(5, 2, NULL);
    snake_segment *sn1 = new_head(4, 2, sn2);
    snake_segment *sn0 = new_head(3, 2, sn1);

    // initilise snakes values
    s->rows = 15;
    s->cols = 20;
    s->head = sn0;
    s->tail = sn2;

    // draw the segments
    // BUG: not getting drawn initially
    attron(COLOR_PAIR(SNAKE));

    for (snake_segment *sptr = s->head;
         sptr;
         sptr = sptr->next)
    {
        mvaddstr(sptr->y, 2 * sptr->x, PIECE);
    }

    attron(COLOR_PAIR(SNAKE));

    // flush buffer
    refresh();

    // return the snake
    return s;
}

void update_snake(snake *s, DIRECTION direction)
{
    // error gurad
    if (!s || !(s->tail))
    {
        // TODO: add error here
        return;
    }

    // add new head according to direction
    int new_x, new_y;

    switch (direction)
    {
    case N:
        new_x = s->head->x;
        new_y = s->head->y - 1;
        break;

    case E:
        new_x = s->head->x + 1;
        new_y = s->head->y;
        break;

    case S:
        new_x = s->head->x;
        new_y = s->head->y + 1;
        break;

    case W:
        new_x = s->head->x - 1;
        new_y = s->head->y;
        break;
    default:
        // TODO: error here error no maybe?
        // for now end function
        return;
    }

    // add head
    snake_segment *head = new_head(new_x, new_y, s->head);
    s->head = head;

    // draw head
    attron(COLOR_PAIR(SNAKE));
    mvaddstr(s->head->y, 2 * s->head->x, PIECE);
    attroff(COLOR_PAIR(SNAKE));

    // undrawn tail
    attron(COLOR_PAIR(BACKGROUND));
    mvaddstr(s->tail->y, (2 * s->tail->x), PIECE);
    attroff(COLOR_PAIR(BACKGROUND));

    // remove tail
    snake_segment *new_last = s->head;

    for (new_last = s->head; new_last && new_last->next &&
            new_last->next->next;
        new_last = new_last->next);

    free(s->tail);

    // udpate tail
    s->tail = new_last;
    s->tail->next = NULL;

    // draw window
    refresh();
}

snake_segment *new_head(int16_t x, int16_t y, snake_segment *next)
{
    // allocate the segments memory
    snake_segment *new = malloc(sizeof(snake_segment));

    // set parameters
    new->x = x;
    new->y = y;
    new->next = next;

    // return the segment
    return new;
}

void add_food(snake *s)
{
    // set position
    s->food_x = rand() % s->cols;
    s->food_y = rand() % s->rows;

    // draw food
    attron(COLOR_PAIR(FOOD));
    mvaddstr(s->food_y, 2 * s->food_x, PIECE);
    attroff(COLOR_PAIR(FOOD));

    // flush screen
    refresh();
}