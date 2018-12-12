#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <ncurses.h>

#include "snake.h"

int main(int argc, char **argv)
{
    init_prog();

    // initilise the game
    game *g = init_game();
    init_board(g);
    add_food(g);

    refresh();

    mainloop(g);

    exit(EXIT_FAILURE);
}

void mainloop(game *g)
{
    // get initial direction to move in
    char c = fgetc(stdin);

    // movement loop
    do
    {
        // deal with input accordingly
        switch (c)
        {
        case 'd':
        case 'a':
        case 'w':
        case 's':
            update_snake(g, c);
            break;
        case 'q':
            quit(g);
            break;
        case 'p':
        case 'e':
            pause_game();
            break;
        }
        update_food(g);

        refresh();

        timeout(75);

        // update movement
        char new_c;
        switch (new_c = getch())
        {
        case 'a':
        case 's':
        case 'd':
        case 'w':
        case 'q':
        case 'p':
        case 'e':
            c = new_c;
            break;
        }

    } while (true);
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

void init_board(game *g)
{
    // draw background/board
    // TODO: optimise this
    attron(COLOR_PAIR(BACKGROUND));
    move(0, 0);
    for (int i = 0; i < g->rows; i++)
    {
        for (int j = 0; j < g->cols; j++)
        {
            addstr(PIECE);
        }

        addch('\n');
    }

    attroff(COLOR_PAIR(BACKGROUND));
}

game *init_game(void)
{
    // declare snake'g memory
    game *g = malloc(sizeof(game));

    /// TODO: make random
    // initilise segments of the snake'g body
    snake_segment *seg = new_segment(5, 2, NULL);

    // initilise snakes values
    getmaxyx(stdscr, g->rows, g->cols);
    g->cols /= 2;
    g->head = seg;
    g->tail = seg;
    g->grow = 0;
    g->score = 1;

    // draw the segments
    // BUG: not getting drawn initially
    attron(COLOR_PAIR(SNAKE));

    for (snake_segment *sptr = g->head;
         sptr;
         sptr = sptr->next)
    {
        mvaddstr(sptr->y, 2 * sptr->x, PIECE);
    }

    attroff(COLOR_PAIR(SNAKE));

    // return the game
    return g;
}

void update_food(game *g)
{
    // check food eaten
    if (g->food_x == g->head->x &&
        g->food_y == g->head->y)
    {
        add_food(g);
        g->grow += 2;
        g->score += 2;
    }
}

bool is_collided(game *g)
{
    //check border collisions
    if (g->head->x < 0 || g->head->x >= g->cols ||
        g->head->y < 0 || g->head->y >= g->cols)
    {
        return true;
    }

    // iterate snake and check collisions
    for (snake_segment *head = g->head,
                       *seg = g->head->next ? g->head->next : g->head;
         seg->next; seg = seg->next)
    {
        if (head->x == seg->x && head->y == seg->y)
        {
            return true;
        }
    }

    return false;
}

void update_snake(game *g, DIRECTION direction)
{
    // error gurad
    if (!g || !(g->tail))
    {
        // TODO: add error here
        return;
    }

    // add new head according to direction
    int new_x, new_y;

    switch (direction)
    {
    case N:
        new_x = g->head->x;
        new_y = g->head->y - 1;
        break;

    case E:
        new_x = g->head->x + 1;
        new_y = g->head->y;
        break;

    case S:
        new_x = g->head->x;
        new_y = g->head->y + 1;
        break;

    case W:
        new_x = g->head->x - 1;
        new_y = g->head->y;
        break;
    default:
        // TODO: error here error no maybe?
        // for now end function
        return;
    }

    // ignore opposite change of directions
    if (g->head->next && new_x == g->head->next->x && new_y == g->head->next->y)
    {
        switch (direction)
        {
        case N:
            update_snake(g, S);
            break;
        case S:
            update_snake(g, N);
            break;
        case E:
            update_snake(g, W);
            break;
        case W:
            update_snake(g, E);
            break;
        }
        return;
    }

    // add head
    snake_segment *head = new_segment(new_x, new_y, g->head);
    g->head = head;

    if (is_collided(g))
    {
        has_collided(g);
    }

    // draw head
    attron(COLOR_PAIR(SNAKE));
    mvaddstr(g->head->y, 2 * g->head->x, PIECE);
    attroff(COLOR_PAIR(SNAKE));

    // undrawn tail
    if (g->grow < 1)
    {
        attron(COLOR_PAIR(BACKGROUND));
        mvaddstr(g->tail->y, (2 * g->tail->x), PIECE);
        attroff(COLOR_PAIR(BACKGROUND));

        // remove tail
        snake_segment *new_last = g->head;

        for (new_last = g->head; new_last && new_last->next &&
                                 new_last->next->next;
             new_last = new_last->next)
            ;

        free(g->tail);

        // udpate tail
        g->tail = new_last;
        g->tail->next = NULL;
    }
    else
    {
        g->grow--;
    }
}

snake_segment *new_segment(int16_t x, int16_t y, snake_segment *next)
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

void add_food(game *g)
{
    // set position
    g->food_x = rand() % g->cols;
    g->food_y = rand() % g->rows;

    // draw food
    attron(COLOR_PAIR(FOOD));
    mvaddstr(g->food_y, 2 * g->food_x, PIECE);
    attroff(COLOR_PAIR(FOOD));
}

void has_collided(game *g)
{
    mvprintw(g->rows / 2, 2 * (g->cols / 2) - 30 / 2,
             "You died. Riiiiip. Score: %d", g->score);
    timeout(10000);
    getch();
    quit(g);
}

void pause_game()
{
    // TODO: print pretty pause screen
    timeout(0);
    getch();
}

void quit(game *g)
{
    // TODO: free memory
    exit(EXIT_SUCCESS);
}