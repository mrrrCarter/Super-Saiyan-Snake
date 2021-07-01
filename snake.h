// Copywright 2021 Carther Theogene carther@bu.edu
#include <iostream>
#include <vector>
#include <ncurses.h>  //  API we use when working with raw terminal code. It provides functions to move the cursor, create windows, produce colors, play with mouse etc
#include <cstdlib>

// macro
#ifndef SNAKE_H
#define SNAKE_H

struct snakepart
{
        int x,y;
        snakepart(int col, int row);
        snakepart();
};

class snakeclass
{
int points, delay, maxwidth, maxheight;
char direction, partchar, screenedgechar, foo, obstacle;
bool get;
snakepart food;
std::vector<snakepart> snake;

void addfood();
bool collision();  // true if touch the edge or if eat food or if snake touch self
void movesnake();

public:  // we cant interact with class without the public
snakeclass(); // constructor
~snakeclass(); // destructor

void start(); //  where we'll start the game

};


#endif