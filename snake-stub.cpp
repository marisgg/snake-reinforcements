
#include <curses.h>
#include <stdio.h>
#include <terminalfunc.h>
#include <unistd.h>

#include <iostream>
#include <random>

bool gameover;
const int width = 30;
const int height = 15;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

const char VERTICAL_WALL = '|';
const char HORIZONTAL_WALL = '-';
const char SNAKE = 'o';
const char FRUIT = '%';
const char SPACE = ' ';

const char cLEFT = 'a';
const char cRIGHT = 'd';
const char cUP = 'w';
const char cDOWN = 's';
const char cTERMINATE = 'x';

using u32 = uint_least32_t;
using engine = std::mt19937;

std::random_device os_seed;
const u32 seed = os_seed();

engine generator(seed);
std::uniform_int_distribution<u32> distributeX(0, width);
std::uniform_int_distribution<u32> distributeY(0, height);

enum eDirection {
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};  // Controls

eDirection dir;
eDirection prev_dir;

void Setup() {
    gameover = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = distributeX(generator);
    fruitY = distributeY(generator);
    score = 0;
}

void _draw_horizontal_border() {
    for (int i = 0; i < width + 2; i++) {
        std::cout << HORIZONTAL_WALL;
    }

    std::cout << std::endl;
}

void Draw(const bool DRAW_HOR_BORDER) {
    system("clear");

    if (DRAW_HOR_BORDER) {
        _draw_horizontal_border();
    }

    for (int i = 0; i <= height; i++) {
        for (int j = 0; j <= width; j++) {
            if (j == 0 || j == width) {
                std::cout << VERTICAL_WALL;
            }

            if (i == y && j == x) {
                std::cout << SNAKE;  // snake tail
            } else if (i == fruitY && j == fruitX) {
                std::cout << FRUIT;
            } else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        std::cout << SNAKE;
                        print = true;
                    }
                }
                if (!print)
                    std::cout << SPACE;
            }
        }

        std::cout << std::endl;
    }

    if (DRAW_HOR_BORDER) {
        _draw_horizontal_border();
    }

    std::cout << std::endl
              << "Score:" << score << std::endl;
}

void Input() {
    if (_kbhit()) {
        prev_dir = dir;
        switch (_getch()) {
            case cLEFT:
                if (prev_dir != RIGHT) {
                    dir = LEFT;
                }
                break;
            case cRIGHT:
                if (prev_dir != LEFT) {
                    dir = RIGHT;
                }
                break;
            case cUP:
                if (prev_dir != DOWN) {
                    dir = UP;
                }
                break;
            case cDOWN:
                if (prev_dir != UP) {
                    dir = DOWN;
                }
                break;
            case cTERMINATE:
                gameover = true;
                break;
        }
    }
}

void Algorithm() {
    int prevX = tailX[0];
    int prevY = tailY[0];

    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];

        tailX[i] = prevX;
        tailY[i] = prevY;

        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x >= width)
        x = 0;
    else if (x < 0)
        x = width - 1;

    if (y > height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameover = true;
    }

    if (x == fruitX && y == fruitY) {
        score += 1;
        fruitX = distributeX(generator);
        fruitY = distributeY(generator);
        nTail++;
    }
}

int main() {
    Setup();

    while (!gameover) {
        Draw(false);
        Input();
        Algorithm();
        usleep(50000);
    }

    return score;
}
