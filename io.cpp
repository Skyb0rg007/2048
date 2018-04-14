/* Comp 11
 * Homework 7
 * Skye Soss
 */


#include "io.h"
#include <termios.h>//termios, tcgetattr(), tcsetattr(), .c_lflag()
#include <unistd.h> //STDIN_FILENO                 -- for buffer settings
#include <string>
#include <iostream>

using namespace std;

//internal prototypes
static char arrow_to_char (char, char, char);
static void shift_down    (char arr[5]);
static void shift_up      (char arr[5]);

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

void modify_buffer(int mode)
{
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);//Get the current terminal I/O structure
  if (mode == GAME_MODE)
    t.c_lflag &= ~(ICANON | ECHO);       //Disable buffering and echo
  else
    t.c_lflag |= (ICANON | ECHO);        //Enable buffering and echo

  tcsetattr(STDIN_FILENO, TCSANOW, &t); //Apply the new settings now
}

char get_input(string valid_keys)   //Validates input
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

int get_difficulty()
{
  char select[5] = {' ', ' ', '>', ' ', ' '}; //Default position: 2
  char inp;
  do {
    cout << "\033[2J"; //clear screen
    cout << select[0] << " Puny     \t(2x2)\n"  //Printing the difficulties
         << select[1] << " Small    \t(3x3)\n"
         << select[2] << " Medium   \t(4x4)\n"
         << select[3] << " Large    \t(5x5)\n"
         << select[4] << " Massive  \t(6x6)\n\n"
         "Use 'wasd' or arrow keys to move\n"
         "Press ENTER to select the difficulty\n"
         "\nIngame:\n"
         "'q' to quit the game\n"
         "'h' to view scores\n";
    inp = cin.get();    //Extracts one character from cin

    if (inp == 27) {    //This if statement changes arrow keys to wasd
      char b = cin.get();
      char c = cin.get();
      inp = arrow_to_char(inp, b, c);
    }     //read my arrow_to_char() comments if this looks weird

    if (inp == 'w')
      shift_up(select);   //W moves up
    if (inp == 's')
      shift_down(select); //S moves down

  } while (inp != '\n');  //pressing enter ends the loop
  for (int i = 0; i < 5; i++)
    if (select[i] == '>')
      return i;           //return the location of the pointer

  return 2; //if everything doesn't work out, revert to default
}

static void shift_up(char arr[5]) //helper function for get_difficulty
{ //Moves the pointer to space above where it was
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

