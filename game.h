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
  Game(std::string);      //Contructor (needs filename of scores file)      [x]
 ~Game(void);             //Deconstructor                                   [x]
  void setup_grid(int);   //Creates the grid, using the size given          [x]
  void turn(char);        //takes a character and does the desired move     [x]
  bool is_over(void);     //returns whether game is over                    [x]

//Following functions Should be private, but spec says all funcs must be public

  void get_scores(void);  //Uses filename to grab the top 5 scores          [x]
                          //and load them into the high_scores array
  void show_scores(void); //displays the top 5 highest scores               [x]
  void exit_game(void);   //saves score to file                             [x]

private:
  std::string filename;   //filename for the high scores                    [x]
  pair<std::string, int> high_scores[5];
                          //array of name-scores pairs                      [x]
  Grid my_grid;           //the board itself                                [x]
  bool game_over;         //whether the game is over                        [x]
};

#endif
