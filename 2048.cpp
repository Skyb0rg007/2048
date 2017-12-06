/* Comp11
 * Homework 7
 * Skye Soss
 */

#ifndef __2048_CPP__
#define __2048_CPP__

//For debugging
#define NLOG_2048
#ifndef NLOG_2048
#define LG(x) do { clog << "* Log: " x << endl; } while (0)
#else
#define LG(x) (void) 0
#endif

#include "game.cpp" //Game methods
#include <iostream> //cin, cout, >>, <<

//static prototypes
static char get_input(string);
static char arrow_to_char(char, char, char);
static int get_difficulty();
static void shift_down(char arr[4]);
static void shift_up(char arr[4]);

static char arrow_to_char(char a, char b, char c)
{
  if (a != 27 || b != 91) { //Arrow keys are 3 chars:
    return 'x';             //all four start with 27:91
  }
  switch (c) {
    case 65:
      return 'w';           //up is 27:91:65
    case 66:
      return 's';           //...and so forth
    case 67:
      return 'd';
    case 68:
      return 'a';
    default:
      return 'x';           //in case something weird happens
  }                         //(Will recycle in get_input() func)
}

static char get_input(string valid_keys = "wasdqh")   //Validates input
{
  modify_buffer(GAME_MODE);  //Arrow key input only works without buffering
  char a, b, c, return_val = 'x';
  while (valid_keys.find(return_val) == string::npos) {
      //The loop continues as long as the action isn't listed
    cin >> a;       //Arrow keys are 3 characters long
    if (a == 27) {  //Starting with 27
      cin >> b;
      cin >> c;
      return_val = arrow_to_char(a, b, c);//They need special treatment
    }
    else
      return_val = a; //If not an arrow key, return the first char
  }
  return return_val;
}

static int get_difficulty()
{
  modify_buffer(GAME_MODE);
  char select[5] = {' ', ' ', '>', ' ', ' '};
  char inp;
  do {
    cout << "\033[2J"; //clear screen
    cout << select[0] << " Puny     \t(2x2)\n"
         << select[1] << " Small    \t(3x3)\n"
         << select[2] << " Medium   \t(4x4)\n"
         << select[3] << " Large    \t(5x5)\n"
         << select[4] << " Challenge\t(\?\?\?)\n";
    inp = cin.get();
    if (inp == 27) {
      char b = cin.get();
      char c = cin.get();
      inp = arrow_to_char(inp, b, c);
    }
    if (inp == '\n')
      break;
    if (inp == 'w')
      shift_up(select);
    if (inp == 's')
      shift_down(select);
  } while (true);
  for (int i = 0; i < 5; i++)
    if (select[i] == '>')
      return i;
  return 2; //if everything doesn't work out, revert to default
}

static void shift_up(char arr[5])
{
  if (arr[0] == '>')
    return;
  for (int i = 1; i < 5; i++)
    if (arr[i] == '>') {
      arr[i] = ' ';
      arr[i-1] = '>';
    }
}

static void shift_down(char arr[5])
{
  if (arr[4] == '>')
    return;
  for (int i = 4; i >= 0; i--)
    if (arr[i] == '>') {
      arr[i] = ' ';
      arr[i+1] = '>';
    }
}

int main()
{
  modify_buffer(GAME_MODE);
  int diff = get_difficulty();
  int size = diff + 2;
  Game my_game ("highscores"+int_to_string(diff)+".txt");
  my_game.setup_grid(size);
  char move;
  while (!my_game.is_over()) {
    move = get_input();
    my_game.turn(move);
  }
  return 0;
}

#endif
