/* Comp11
 * Homework 7
 * Skye Soss
 */

 
#include "game.h" //Game methods
#include "io.h"   //IO functions
#include <unistd.h>

int main()
{
  modify_buffer(GAME_MODE);  //Engage GAME_MODE! (makes input work. See io.cpp)

  int diff = get_difficulty(); //kinda obvious what this does
  int size = diff + 2; //Just how the difficulties line up
  Game my_game ("highscores" + int_to_string(diff) + ".txt");
    //The scores file for each difficulty is just
    //a file with the difficulty appended to the name
  my_game.setup_grid(size);

  while (!my_game.is_over())
    my_game.turn(get_input());
    //Keep getting input and doing the turn, until you lose!

  return 0;
}

