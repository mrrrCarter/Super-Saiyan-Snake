// Copywright 2021 Carther Theogene carther@bu.edu
#include "snake.h"
#include <unistd.h>
#include <vector>
using namespace std;

/*
TODO:
Main Menu: Start, Record

*/
snakepart::snakepart(int col, int row) {  //  Defining the coordinates
  x = col;
  y = row;
}

snakepart::snakepart() { 
  x = 0;
  y = 0;
}
snakeclass::snakeclass() {
  initscr();
  nodelay(stdscr, true);          //if there wasn't any key pressed don't wait for keypress
  keypad(stdscr, true);           //init the keyboard
  noecho();                                   //don't write over the terminal
  curs_set(0);                            //cursor invisible
  getmaxyx(stdscr, maxheight, maxwidth);
  partchar = 'x'; //  what the snake will be made of
  screenedgechar = (char)219;
  foo = '*';
  obstacle = '#';
  food.x = 0;
  food.y = 0;
  for(int i = 0; i < 5; i++)
    snake.push_back(snakepart(40 + i, 10));
  points = 0;
  delay = 110000; //  set the speed the snake will be going at
  get = 0;
  direction = 'l';
  srand(time(NULL));
  addfood();
  //make the game-board -- up-vertical
  for(int i = 0; i < maxwidth - 1; i++) {
    move(0, i);
    addch(screenedgechar);
  }
  //left-horizontal
  for(int i = 0; i < maxheight - 1; i++) {
    move(i, 0);
    addch(screenedgechar);
  }
  //down-vertical
  for(int i = 0; i < maxwidth - 1; i++) {
    move(maxheight - 2, i);
    addch(screenedgechar);
  }
  //right-horizontal
  for(int i = 0; i < maxheight - 1; i++) {
    move(i, maxwidth - 2);
    addch(screenedgechar);
  }
  //draw the snake
  for(int i = 0; i < snake.size(); i++) {
    move(snake[i].y, snake[i].x); //  ncurses to place the snake at the initial pos
    addch(partchar);
  }
  move(maxheight - 1, 0); //  where to print the points ( bottom left)
  printw("%d", points);
  move(food.y, food.x); // move the food(l69) at random location(l70) then refresh(l71)
  addch(foo);
  refresh();
}

snakeclass::~snakeclass() {
  nodelay(stdscr, false);         //turn back
  getch();                                        //wait until a key is pressed
  endwin();
}

void snakeclass::addfood() {  //  place the food at random location
  while(1) {
    int tmpx = rand() % maxwidth + 1;
    int tmpy = rand() % maxheight + 1;
    for(int i = 0; i < snake.size(); i++)
      if(snake[i].x == tmpx && snake[i].y == tmpy)  //  Make sure the food is not being place where the snake is
        continue;
    if(tmpx >= maxwidth - 2 || tmpy >= maxheight - 3) //  make sure food is place within our board
      continue;
    food.x = tmpx;
    food.y = tmpy;
    break;
  }
  move(food.y, food.x);
  addch(foo);
  refresh();
}

bool snakeclass::collision() { // Determine what happens during any type of collision
  if(snake[0].x == 0 || snake[0].x == maxwidth - 1 || snake[0].y == 0 || snake[0].y == maxheight - 2) //  if the head of the snake touches any boundaries within the board, YES
    return true;
  for(int i = 2; i < snake.size(); i++) { //  go through the actual snake
    if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) //  make sure it doesnt touch on itself
      return true;
  }
  //collision with the food
  if(snake[0].x == food.x && snake[0].y == food.y) {
    get = true;
    addfood();
    points += 10;
    move(maxheight - 1, 0);
    printw("%d", points);
    if((points % 30) == 0)
      delay -= 5000;
    // move((maxheight / 2) - 4, (maxwidth / 2 + 4));
    // printw("Level Up!");
    // refresh();
  } else
    get = false;
  return false;
}


void snakeclass::movesnake() {
  //detect key
  int tmp = getch();
  switch(tmp) {
  case KEY_LEFT:
    if(direction != 'r')
      direction = 'l';
    break;
  case KEY_UP:
    if(direction != 'd')
      direction = 'u';
    break;
  case KEY_DOWN:
    if(direction != 'u')
      direction = 'd';
    break;
  case KEY_RIGHT:
    if(direction != 'l')
      direction = 'r';
    break;
  case KEY_BACKSPACE:
    direction = 'q';
    break;
  case (' '): //Pause with the backspace
    getchar();
    break;
  }
  //if there wasn't a collision with food
  if(!get) { //  this is how we make the snake move, the idea is to keep removing the last element in the vector, effectively reducing the container size by one.  prepend a head and erase a tail segment. 
    move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
    printw(" ");
    refresh();
    snake.pop_back();
  }
  if(direction == 'l') { //  if we go left then we restart the processes left
    snake.insert(snake.begin(), snakepart(snake[0].x - 1, snake[0].y)); //possible overload might be found doing this expesive operation of inserting a copy of the given value before the specified location. Might be looking at std::list if we want to do this operation a lot.
  } else if(direction == 'r') {
    snake.insert(snake.begin(), snakepart(snake[0].x + 1, snake[0].y));

  } else if(direction == 'u') {
    snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y - 1));
  } else if(direction == 'd') {
    snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y + 1));
  }
  move(snake[0].y, snake[0].x);
  addch(partchar);
  refresh();
}

void snakeclass::start() { //  this is where we call the movesnake function in case of no collision
  while(1) {
    if(collision()) {
      move((maxheight / 2) - 4, (maxwidth / 2)-15);
      printw("GAME OVER! Your final score is: ");
      printw("%d", points);
      break;
    }
    movesnake();
    if(direction == 'q')            //exit
      break;
    usleep(delay);            //Linux delay
  }
}
