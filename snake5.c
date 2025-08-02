#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#define SIZE 16384

enum {UP = -1, DOWN = 1, LEFT = -1, RIGHT = 1};

void spawn_food();
void spawn_snake();
void game();
void game_over();

char mvHisY[SIZE]; // move history
char mvHisX[SIZE];
int tempY = 0;
int tempX = 0;

void refresh_win(WINDOW* win) {
    refresh();
    wrefresh(win);
}

void init_win() {
    initscr();
    WINDOW *win = newwin(25, 75, 0, 0);
    int row, col;
    getmaxyx(win, row, col);
    box(win, 0, 0);
    mvwprintw(win, row / 2, (col / 2) - (strlen("Press any key to start") / 2), "Press any key to start");
    curs_set(0);
    refresh_win(win);
    noecho();
    getch();
    mvwprintw(win, row / 2, col / 2 - (strlen("Press any key to start") / 2), "                      ");
    curs_set(0);

    spawn_food(win, row, col);
}

void spawn_food(WINDOW* win, int row, int col) {
    int foodY = rand() % (row-2) + 1;
    int foodX = rand() % (col-2) + 1;
    mvwaddch(win, foodY, foodX, '@');
    refresh_win(win);

    spawn_snake(win, row, col, foodY, foodX);
}

void respawn_food(WINDOW* win, int* foodY, int* foodX, int row, int col) {
    mvwaddch(win, *foodY, *foodX, ' ');
    *foodY = rand() % (row-2) + 1;
    *foodX = rand() % (col-2) + 1;
    mvwaddch(win, *foodY, *foodX, '@');
}

void spawn_snake(WINDOW* win, int row, int col, int foodY, int foodX) {
    int snakeY = rand() % (row-2) + 1;
    int snakeX = rand() % (col-2) + 1;
    mvwaddch(win, snakeY, snakeX, 'O');
    refresh_win(win);

    game(win, snakeY, snakeX, row, col, foodY, foodX);
}

void game(WINDOW* win, int snakeY, int snakeX, int row, int col, int foodY, int foodX) {
    keypad(win, true);
    mvHisY[0] = snakeY, mvHisX[0] = snakeX;
    int snakeLen = 1;
    int i = 0;
    int over = 0;
    while (!over) {
        int ch = wgetch(win);
        switch (ch) {
            case KEY_UP: snakeY += UP; break;
            case KEY_DOWN: snakeY += DOWN; break;
            case KEY_LEFT: snakeX += LEFT; break;
            case KEY_RIGHT: snakeX += RIGHT; break;
        }
        chtype char_at_head = mvwinch(win, snakeY, snakeX) & A_CHARTEXT;
        if (char_at_head == 'O') {
            over = 1;
            game_over(win, row, col);
        }
        ++i; // --------------------------------------------------------> spent 3 days
        mvHisY[i] = snakeY, mvHisX[i] = snakeX; // ---------------------> to write
        mvwaddch(win, mvHisY[i-snakeLen], mvHisX[i-snakeLen], ' '); // -> these 3 lines of code
        if (snakeY < 1 || snakeY > (row-2) || snakeX < 1 || snakeX > (col-2)) { // borders
            over = 1;
            game_over(win, row, col);
        }
        if (snakeY == foodY && snakeX == foodX) {
            ++snakeLen;
            respawn_food(win, &foodY, &foodX, row, col);
        }
        mvwaddch(win, snakeY, snakeX, 'O');
        mvprintw(25, 0, "mvHisY[%d] = %d\nmvHisX[%d] = %d\n", i, mvHisY[i], i, mvHisX[i]);
        mvprintw(27, 0, "snakeY = %d\nsnakeX = %d\ni = %d\nsnakeLen = %d\
        \nmoveHistoryY[0] = %d\nmoveHistoryX[0] = %d\nmoveHistoryY[1] = %d\nmoveHistoryX[1] = %d\ntempY = %d\ntempX = %d\n", 
        snakeY, snakeX, i, snakeLen, mvHisY[0], mvHisX[0], mvHisY[1], mvHisX[1], tempY, tempX);
        refresh_win(win);
    }
    sleep(5);
}

void game_over(WINDOW* win, int row, int col) {
    for (int i = 1; i < row-1; ++i) {
        for (int j = 1; j < col-1; ++j) {
            mvwprintw(win, i, j, " ");
        }
    }
    mvwprintw(win, row / 2, (col / 2) - (strlen("Game Over!") / 2), "Game Over!");
    refresh_win(win);
}


int main(void) {
    srand(time(NULL));
    init_win();
    endwin();
    return 0;
}