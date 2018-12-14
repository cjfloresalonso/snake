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
    init_board(NULL);
    game *g = init_game();
    add_food(g);

    refresh();

    mainloop(g);

    exit(EXIT_FAILURE);
}

/******************************
 * Model/Controller Fucntions *
 *****************************/
void mainloop(game *g)
{
    // get initial direction to move in
    int32_t prev_move = '\0';

    // movement loop
    do
    {
        // update movement
        timeout(75);
        int32_t c = getch();


        // translate alias keys
        switch (c)
        {
        case 'w':
            c = KEY_UP;
            break;
        case 's':
            c = KEY_DOWN;
            break;
        case 'a':
            c = KEY_LEFT;
            break;
        case 'd':
            c = KEY_RIGHT;
            break;
        }

        switch (c)
        {
        case KEY_LEFT:
        case KEY_DOWN:
        case KEY_RIGHT:
        case KEY_UP:
            prev_move = c;
            break;
        case 'q':
        case 'p':
            break;
        default:
            c = prev_move;
        }


        // deal with input accordingly
        switch (c)
        {
        case KEY_RIGHT:
        case KEY_LEFT:
        case KEY_UP:
        case KEY_DOWN:
            update_snake(g, c);
            break;
        case 'q':
            quit(g);
            break;
        case 'p':
        case 'e':
            pause_game(g);
            break;
        }

        // check and update food accordingly
        update_food(g);

        // draw the game
        refresh();

    } while (true);
}

void init_prog(void)
{
    // initilise the tui
    initscr();
    refresh();

    cbreak();
    noecho();
    keypad(stdscr, TRUE);

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

game *init_game(void)
{
    // declare snake'g memory
    game *g = malloc(sizeof(game));

    // initilise snakes values
    getmaxyx(stdscr, g->rows, g->cols);
    g->cols /= 2;

    // initilise segments of the snake'g body
    snake_segment *seg = new_segment(
        rand() % g->cols,
        rand() % g->rows,
        NULL);

    g->head = seg;
    g->tail = seg;

    g->grow = 0;

    g->score = 1;
    g->start = time(NULL);

    // draw the segment
    draw_snake(g->head, -1, SNAKE);

    // return the game
    return g;
}

void update_food(game *g)
{
    // check food eaten
    if (g->food_x == g->head->x &&
        g->food_y == g->head->y)
    {
        // readd food
        add_food(g);

        // udpate game state
        g->grow += 2;
        g->score += 2;
    }
}

bool is_collided(game *g)
{
    //check border collisions
    if (g->head->x < 0 || g->head->x >= g->cols ||
        g->head->y < 0 || g->head->y >= g->rows)
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

    // deal with collisions
    if (is_collided(g))
    {
        has_collided(g);
    }

    // draw head
    draw_snake(g->head, 1, SNAKE);

    // undrawn tail
    if (g->grow < 1)
    {
        draw_snake(g->tail, 1, BACKGROUND);

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
    // get new food positions
    uint16_t food_x = rand() % g->cols;
    uint16_t food_y = rand() % g->rows;

    // ensure food not drawn on snake
    for (snake_segment *s = g->head; s->next; s = s->next)
    {
        if (food_x == s->x &&
            food_y == s->y)
        {
            add_food(g);
            return;
        }
    }

    // set position
    g->food_x = food_x;
    g->food_y = food_y;

    // draw food
    draw_food(g);
}

void has_collided(game *g)
{
    // print death screen
    attron(COLOR_PAIR(TEXT));
    mvprintw(g->rows / 2, 2 * (g->cols / 2) - 30 / 2,
             "You died. Riiiiip. Score: %d", g->score);
    attroff(COLOR_PAIR(TEXT));

    refresh();
    timeout(10000);
    getch();

    quit(g);
}

int64_t time_elapsed(game *g)
{
    if (!g || !g->start)
    {
        // TODO:  some sort of error here
        exit(1);
    }

    return (int64_t)(time(NULL) - g->start);
}

void pause_game(game *g)
{
    // get time paused
    time_t time_paused = time(NULL);

    WINDOW *pause_window = new_pause_window(g);

    // show pause screen
    wrefresh(pause_window);

    // wait for unpause key
    timeout(-1);
    int c;
    while ((c = getch()))
    {
        if (c == 'p')
        {
            break;
        }
        else if (c == 'q')
        {
            quit(g);
        }
    }

    kill_pause_window(pause_window);

    // restore game
    wrefresh(stdscr);

    // remove time paused from start
    g->start += time(NULL) - time_paused;
}

void free_game(game *g)
{
    if (!g)
    {
        return;
    }

    // free memory
    snake_segment *prev = g->head;
    snake_segment *sptr = prev->next;

    while (sptr)
    {
        free(prev);
        prev = sptr;
        sptr = sptr->next;
    }

    free(prev);

    g = NULL;
}

void quit(game *g)
{
    free_game(g);

    exit(EXIT_SUCCESS);
}

/******************
 * View Functions *
 *****************/

void init_board(game *g)
{
    // set background colour
    bkgd(COLOR_PAIR(BACKGROUND));

    // darken last column if applicable
    attron(COLOR_PAIR(TEXT));
    for (int i = 0, width = getmaxx(stdscr), odd = width % 2;
         odd && i < getmaxy(stdscr); i++)
    {
        mvaddch(i, width - 1, ' ');
    }
    attroff(COLOR_PAIR(TEXT));
}

void draw_snake(snake_segment *head, int16_t count, int32_t colour)
{
    // error gaurd
    if (!head)
    {
        // some kind error here
        return;
    }

    // set colour
    attron(COLOR_PAIR(colour));

    // TODO: check for an error

    // iterate snake and draw segments
    snake_segment *s = head;
    for (int32_t i = 0; s && (count < 0 || i < count); s = s->next, i++)
    {
        mvaddstr(s->y, 2 * s->x, PIECE);
    }

    // unset colour
    attroff(COLOR_PAIR(colour));
}

void draw_food(game *g)
{
    // draw game's food
    attron(COLOR_PAIR(FOOD));
    mvaddstr(g->food_y, 2 * g->food_x, PIECE);
    attroff(COLOR_PAIR(FOOD));
}

WINDOW *new_pause_window(game *g)
{
    // create and configure pause screen
    WINDOW *pause_window = newwin(
        10, 40,
        getmaxy(stdscr) / 2 - 10 / 2,
        getmaxx(stdscr) / 2 - 40 / 2);

    wbkgd(pause_window, COLOR_PAIR(TEXT));
    werase(pause_window);

    // write message to screen
    mvwaddstr(pause_window, 1, 1, "  __                ___               \n");
    mvwaddstr(pause_window, 2, 1, " / _| _    _ _  _  | o \\ _    _  _  ||\n");
    mvwaddstr(pause_window, 3, 1,
              "( |_n/o\\ |/ \\ \\/o\\ |  _//o\\|U(c'/o\\/o|\n");
    mvwaddstr(pause_window, 4, 1,
              " \\__/\\_,]L_n_n|\\(  |_|  \\_,]_|_)\\( \\_|\n");

    mvwaddstr(pause_window, 7, 3, "Press p to continue");
    mvwaddstr(pause_window, 8, 6, "or q to quit");

    return pause_window;
}

void kill_pause_window(WINDOW *pwindow)
{
    /* 
     * clear pause window
     * by
     * setting the background colour then filling the window with blanks
     */
    wbkgd(pwindow, COLOR_PAIR(BACKGROUND));
    werase(pwindow);

    // show changes
    wrefresh(pwindow);

    // delete pause window
    delwin(pwindow);
}
