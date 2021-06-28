#include "snake.h"
#include <unistd.h>

// snakepart constructor
snakepart::snakepart(int col, int row) {
  x = col;
  y = row;
}

snakepart::snakepart() {  //  empty snake part
  x = 0;
  y = 0;
}

snakeclass::snakeclass() {
  initscr();  //  initialize the screen
  nodelay(stdscr, true);  //  so it doesnt until user press a key
  keypad(stdscr, true); // making it possible to use the keyboard to play
  noecho(); //  to make sure we dont write on the screen if we accidentally press something on the keyboard
  curs_set(0); // to set the cursor to invisible
  getmaxyx(stdscr, maxheight, maxwidth);

  //  here we initialize all the variables we declared in snake.h
  partchar = 'x'; // what the snake will be made of for now
  screenedgechar = (char)219; //  what the edge will be made of (rectangle in ASCII)
  foo = '*'; // the food for now
  food.x = 0; // initial direction of food. will be random after
  food.y = 0;

  for(int i = 0; i < 5; i++) {
    snake.push_back(snakepart(40 + i, 10)); // where we will place the initial snake
    points = 0;
    delay = 110000;
    get = false;
    direction = 'l'; //  where the snake will start heading
    srand(time(0));  //  random number gen
    addfood();

    //  let's make the edge of the board
    for (int i = 0; i < maxwidth - 1; i++) {  //  top
      move(0, i);
      addch(screenedgechar);
    }
    for (int i = 0; i < maxwidth - 1; i++) { //  bottom
      move(maxheight - 2, i);
      addch(screenedgechar);
    }
    for (int j = 0; j < maxheight - 1; j++) {  // left screen
      move(j, 0);
      addch(screenedgechar);
    }
    for (int j = 0; j < maxheight - 1; j++) {  //  right screen
      move(j, maxwidth - 2);
      addch(screenedgechar);
    }

    // draw the snake
    for (int i = 0; i < snake.size(); i++) {
      move(snake[i].y, snake[i].x);
      addch(partchar);
    }
    move(maxheight - 1, 0);
    printw("%d", points);
    move(food.y, food.x);
    addch(foo);
    refresh(); //  that way we dont have to delete and write again
  }

}
snakeclass::~snakeclass() {
  nodelay(stdscr, false);
  getch();
  endwin();
}
void snakeclass::addfood() {
  while(1) { // place it randomly on the board
    int tmpx = rand() % maxwidth + 1;
    int tmpy = rand() % maxheight + 1;
    for (int i = 0; i < snake.size(); i++)
      if (snake[i].x == tmpx && snake[i].y == tmpy) //  making sure the food is not where the snake is
        continue;
    if (tmpx >= maxwidth - 2 || tmpy >= maxwidth - 3) //  making sure the food is not out of bound
      continue;
    food.x = tmpx;
    food.y = tmpy;
    break;

  }
  move(food.y, food.x);
  addch(foo);
  refresh();
}

//  handle collision
bool snakeclass::collision() {
  if (snake[0].x == 0 || snake[0].x == maxwidth - 1 || snake[0].y == 0 || snake[0].y == maxheight - 2)
    return true;

  for (int i = 2; i < snake.size(); i++)
    if (snake[0].x == snake[i].x && snake[i].y == snake[0].y)
      return true;
  if (snake[0].x == food.x && snake[0].y == food.y) {
    get = true;  //  if we collide with the food then one more char will be added to snake
    addfood(); //  new food since snake took last food
    points += 10;
    move(maxheight - 1, 0);
    printw("%d", points);
    // make the snake faster if 100 points increments are reached
    if ((points % 100) == 0)
      delay -= 10000; // a lot less delay
  } else
    get = false;
  return false;
}

//  handle move snake
void snakeclass::movesnake() {
    int key = getch(); //  getch() returns the ASCII value of the character read from stdin
    switch(key) {
        case KEY_LEFT:
            if(direction != 'r')
                direction = 'l';
            break;
        case KEY_UP:
            if(direction != 'd')
                direction = 'u';
            break;
        case KEY_RIGHT:
            if(direction != 'l')
                direction = 'r';
            break;
        case KEY_DOWN:
            if(direction != 'u')
                direction = 'd';
            break;
        case KEY_BACKSPACE:
            direction = 'q';
            break;
    }
    // move the snake (really we're just going to pop back and delete) if the snake goes left and didnt get the food we delete the last element of the left and add an element to the right in a rolling fashion
    if(get) {  //  if we dont get the food
        move(snake[snake.size()-1].x, snake[snake.size()-1].y);  //  move to the end of the snake
        addch(' '); //  so we dont see the last element
        refresh();
        snake.pop_back(); //  removes the last el of the snake
    }
    if (direction == 'l')
        snake.insert(snake.begin(), snakepart(snake[0].x-1, snake[0].y));
    else if (direction == 'r')
        snake.insert(snake.begin(), snakepart(snake[0].x+1, snake[0].y));
    else if (direction == 'u')
        snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y-1));
    else if (direction == 'd')
        snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y+1));
    move(snake[0].y, snake[0].x);
    addch(partchar);
    refresh();
}

void snakeclass::start() {
    while(1) {
        if(collision()) {
            move((maxheight/2)-4, (maxwidth/2)-2);  //  where we'll place "game over" approx the middle of the screen
            printw("GAME OVER!");
            break;
        }
        movesnake();
        if (direction == 'q')
            break;
        usleep(delay);
    }
}