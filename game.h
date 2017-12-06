/* Comp11
 * Homework 6
 * Skye Soss
 * Checked by: Sammy S
 */

#ifndef GAME_H_
#define GAME_H_

#include <string>   //string
#include "grid.h"   //Grid
#include <utility>  //pair

class Game
{
public:
  Game(std::string);   //Contructor (needs filename of scores file)         [x]
  ~Game();             //Deconstructor                                      [x]
  void setup_grid(int);//Creates the grid, using the size given             [x]
  void turn(char);     //takes a character and does the desired move        [x]
                       // (w a s d q h)
  bool is_over();      //returns whether game is over                       [x]
private:
  void get_scores();   //Uses filename to grab the top 5 scores             [x]
                       // and load them into the high_scores array
  void show_scores();  //displays the top 5 highest scores                  [x]
  void exit_game();    //save score                                         [x]
                       //(everythiung else needs to be implemented out of scope)
  std::string filename;//filename for the high scores                       [x]
  pair<std::string, int> high_scores[5];  //array of name-scores pairs      [x]
  Grid my_grid;        //the board itself                                   [x]
  bool game_over;      //whether the game is over                           [x]
};

#endif
