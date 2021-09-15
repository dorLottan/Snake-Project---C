
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

#define LOSE 1
#define WIN 2
#define ROW_MAX_LENGTH 25
#define COL_MAX_LENGTH 75
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define MAXLEN 20

struct Food
{
  int x;
  int y;
  bool eaten;
};

struct Snake
{
  int x[MAXLEN];
  int y[MAXLEN];
  int length;
};

int menu();
void gameOver();
void gameWon();
void gotoxy(int x, int y);
void printWalls();
int playGame();
struct Snake printSnake(struct Snake snake);
struct Food setFood(struct Food food);
bool wallHit(struct Snake snake);
bool selfIntersect(struct Snake snake);
void startPlay();

main()
{
  int level;
  level = menu();
  while (true)
  {
    while (level > 1 || level < 0)
    {
      level = menu();
    }
    if (level == 0)
    {
      return 0;
    }
    else
    {
      system("cls");
      printWalls();
      startPlay();
    }

    level = menu();
  }
}

//This function prints the menu of the game
int menu()
{
  int level;
  printf("*********************************** \n");
  printf("* Pick Your Level, to exit press 0 \n");
  printf("* 0 - Exit \n");
  printf("* 1 - Basic Level \n");
  printf("* 2 - Intermidiate Level \n");
  printf("* 3 - Advanced Level \n");
  printf("*********************************** \n");
  scanf("%d", &level);
  return level;
}

//This function prints the loss message
void gameOver()
{
  system("cls");
  printf("################################### \n");
  printf("#            Game Over            #\n");
  printf("################################### \n");
}

//This function prints the victory message
void gameWon()
{
  system("cls");
  printf("################################### \n");
  printf("#            You Won!!            #\n");
  printf("################################### \n");
}

void gotoxy(int y, int x)
{
  printf("\x1b[%d;%df", x + 1, y + 1);
}

//This function prints the borders of the game
void printWalls()
{
  int i, j;
  for (i = 0; i <= ROW_MAX_LENGTH; i++)
    for (j = 0; j <= COL_MAX_LENGTH; j++)
    {
      gotoxy(0, i);
      printf("#");
      gotoxy(COL_MAX_LENGTH, i);
      printf("#");
      gotoxy(j, 0);
      printf("#");
      gotoxy(j, ROW_MAX_LENGTH);
      printf("#");
    }
}

//The game process,
//the snake moves in the direction of the key that the user typed.
//The goal of the game is to eat 10 food items that are randomly placed on the board
//without the snake hitting the the walls or his tail
int playGame()
{
  struct Food food;
  food.eaten = true;

  struct Snake snake;
  snake.x[0] = 25;
  snake.y[0] = 10;
  snake.length = 10;

  int score = 0;
  int key = _getch();
  int lastKey = -1;

  while (key != ESC)
  {

    while (!_kbhit())
    {

      food = setFood(food);

      for (int i = snake.length; i > 0; i--)
      {
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
      }

      if (key != UP && key != DOWN && key != RIGHT && key != LEFT)
      {
        key = lastKey;
      }
      else
      {
        lastKey = key;
      }

      switch (key)
      {
      case UP:
        snake.y[0] -= 1;
        break;
      case DOWN:
        snake.y[0] += 1;
        break;
      case RIGHT:
        snake.x[0] += 1;
        break;
      case LEFT:
        snake.x[0] -= 1;
        break;
      case ESC:
        break;
      default:
        break;
      }

      printSnake(snake);

      if (selfIntersect(snake))
      {
        return LOSE;
      }

      if (wallHit(snake))
      {
        return LOSE;
      }

      //check if the snake eaten the food
      if (snake.x[0] == food.x && snake.y[0] == food.y)
      {
        gotoxy(food.x, food.y);
        printf(" ");
        snake.length++;
        score++;
        food.eaten = true;
      }

      gotoxy(34, 26);
      printf("SCORE: %d", score);

      if (snake.length == MAXLEN)
      {
        return WIN;
      }
    }
    key = _getch();
  }
  return LOSE;
}

//This function prints the snake on the board
struct Snake printSnake(struct Snake snake)
{
  for (int i = 0; i < snake.length - 1; i++)
  {
    gotoxy(snake.x[i], snake.y[i]);
    printf("@");
  }
  Sleep(100);
  gotoxy(snake.x[snake.length - 1], snake.y[snake.length - 1]);
  printf(" ");
}

//This function places the food randonly on the game board,
//if the food was eaten it sets a new one
struct Food setFood(struct Food food)
{
  if (food.eaten == true)
  {
    srand(time(NULL));
    food.x = 1 + rand() % (COL_MAX_LENGTH - 2);
    food.y = 1 + rand() % (ROW_MAX_LENGTH - 2);
    food.eaten = false;
  }

  if (food.eaten == false)
  {
    gotoxy(food.x, food.y);
    printf("$");
  }

  return food;
}

//This function checkes if the snake hit the wall
bool wallHit(struct Snake snake)
{
  if (snake.x[0] < 1 || snake.x[0] >= COL_MAX_LENGTH || snake.y[0] < 1 || snake.y[0] >= ROW_MAX_LENGTH)
  {
    return true;
  }
  return false;
}

//This functions checks if the snake hit its tail
bool selfIntersect(struct Snake snake)
{
  for (int i = 1; i < snake.length; i++)
  {
    if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
    {
      return true;
    }
  }
  return false;
}

//This function starts the game,
//and prints a victory or loss message after it ends
void startPlay()
{
  int gameState = 0;

  while (gameState == 0)
  {
    gameState = playGame();
  }

  if (gameState == LOSE)
  {
    gameOver();
  }
  else
  {
    gameWon();
  }
}
