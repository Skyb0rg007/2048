/* Comp11
 * Homework 6
 * Skye Soss
 * Checked by: Sammy S
 */

#ifndef __GRID_H__
#define __GRID_H__

class Grid
{
public:
  Grid();                //contructor                                       [x]
  ~Grid();               //deconstructor                                    [x]
  void create_grid(int); //create the board with the size given,            [x]
                         // allocating memory as necessary
  bool move(char);        //moving; returns false if in a losing position   [x]
  void show();           //print the board                                  [x]
  int  get_score();      //returns the current score                        [x]
  void spawn(int);       //create a tile, false if you can't -> you lose    [x]
                         //param if you have a specific value for the tile

private:
  bool check_loss();  //returns true if everything is ok, false if you lost [x]
  bool combine(int, int, int, int); //combine x,y and x1,y1                 [x]
  bool slide(int, int, int, int);   //move x,y to x1,y1 if x1,y1 is empty   [x]
  void left();                      //directional movements                 [x]
  void right();                     //(these move all of the tiles)         [x]
  void up();                        //                                      [x]
  void down();                      //                                      [x]
  void delete_grid(); //deletes the grid and derefrences the grid pointer   [x]
  int ** grid;        //2d array of ints -- value of the board squares      [x]
  int grid_size;      //                                                    [x]
  int score;          //pretty obvious what this holds                      [x]
};

#endif
