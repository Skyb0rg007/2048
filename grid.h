/* Comp11
 * Homework 6
 * Skye Soss
 * Checked by: Sammy S
 */

#ifndef __GRID_H__
#define __GRID_H__

#include <string>

class Grid
{
public:
  Grid(void);            //contructor                                       [x]
 ~Grid(void);            //deconstructor                                    [x]
  void create_grid(int); //create the board with the size given,            [x]
                         // allocating memory as necessary
  bool move(char);       //moving; returns false if in a losing position    [x]
  void show(void);       //print the board                                  [x]
  int  get_score(void);  //returns the current score                        [x]
  void spawn(int);       //create a tile, false if you can't -> you lose    [x]
                         //param if you have a specific value for the tile

//Following functions should be private, but spec says all funcs must be public

  bool check_loss(void); //returns false if you lost                        [x]
  bool combine(int, int, int, int); //combine x,y and x1,y1                 [x]
  bool slide  (int, int, int, int); //move x,y to x1,y1 if x1,y1 is empty   [x]
  void left (void);                 //directional movements                 [x]
  void right(void);                 //(these move all of the tiles)         [x]
  void up   (void);                 //                                      [x]
  void down (void);                 //                                      [x]
  void delete_grid(void); //deletes grid and derefrences the grid pointer   [x]

private:
  int** grid;        //2d array of ints -- value of the board squares       [x]
  int   grid_size;   //the x and y dimensions of the grid                   [x]
  int   score;       //pretty obvious what this holds                       [x]
};

//exported prototypes
std::string int_to_string (int);

#endif
