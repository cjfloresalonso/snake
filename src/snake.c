#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <ncurses.h>

#include "snake.h"

int main(int argc, char **argv)
{
    // init stuff
    initscr();
    atexit((void *)endwin());
    cbreak();
    curs_set(0);
    noecho();
    start_color();
    init_pair(BACKGROUND, COLOR_WHITE, COLOR_WHITE);
    init_pair(SNAKE, COLOR_BLACK, COLOR_GREEN);
    init_pair(FOOD, COLOR_BLACK, COLOR_RED);
    init_pair(TEXT, COLOR_WHITE, COLOR_BLACK);
    srand(time(NULL));

    snake_segment *sn2 = new_head(5, 2, NULL);
    snake_segment *sn1 = new_head(4, 2, sn2);
    snake_segment *sn0 = new_head(3, 2, sn1);
    snake *s = malloc(sizeof(snake));

    s->rows = 15;
    s->cols = 20;
    s->head = sn0;
    s->tail = sn2;

    init_board(s);
    init_snake(s);

    add_food(s);

    do
    {
        attron(COLOR_PAIR(TEXT));
        char c = fgetc(stdin);
        attroff(COLOR_PAIR(TEXT));

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
            draw_end_game();
            endwin();
            return 1;
        case 's':
            update_snake(s, S);
            break;
        default:
            break;
        }

        fflush(stdin);
    } while (true);

    draw_end_game();
    return 0;
}

void init_board(snake *s)
{
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

void init_snake(snake *s)
{
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
}

void update_snake(snake *s, DIRECTION direction)
{
    // error gurad
    if (!s || !(s->tail))
    {
        // TODO: add error here
        return;
    }

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

    // remove tail
    attron(COLOR_PAIR(BACKGROUND));
    mvaddstr(s->tail->y, (2 * s->tail->x), PIECE);
    attroff(COLOR_PAIR(BACKGROUND));

    snake_segment *new_last = s->head;

    while (new_last && new_last->next && new_last->next->next)
    {
        new_last = new_last->next;
    }

    free(s->tail);
    s->tail = new_last;
    s->tail->next = NULL;

    // flush buffer
    refresh();
}

void draw_end_game(void)
{
    addch('\n');
}

snake_segment *new_head(int16_t x, int16_t y, snake_segment *next)
{
    snake_segment *new = malloc(sizeof(snake_segment));

    new->x = x;
    new->y = y;
    new->next = next;

    return new;
}

void add_food(snake *s)
{
    // set position
    s->food_x = rand() % s->cols;
    s->food_y = rand() % s->rows;

    FILE *log = fopen("log.txt", "a");
    fprintf(log, "%d/%d\n%d/%d\n", s->food_x, s->cols, s->food_y, s->rows);
    fclose(log);

    // draw it
    attron(COLOR_PAIR(FOOD));
    mvaddstr(s->food_y, 2 * s->food_x, PIECE);
    attroff(COLOR_PAIR(FOOD));

    refresh();
}