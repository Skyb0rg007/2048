/* Comp11
 * Homework 7
 * Skye Soss
 *
 * NOTE TO GRADER:
 * I liked to think of the grid as an (x,y) system, so that is why
 * the slide and combine functions take arguments as x, y, even though
 * the grid is accessed as grid[y][x].
 */

#ifndef __GRID_CPP__
#define __GRID_CPP__

#include "grid.h"   //Grid prototypes              -- Defined grid class
#include <string>   //string                       -- string modification
#include <iostream> //cout, clog, <<, >>, endl     -- standard I/O
#include <stdlib.h> //srand(), rand()              -- producing random numbers
#include <time.h>   //time()                       -- needed to seed srand()
#include <utility>  //pair, make_pair()            -- storing coordinates
#include <vector>   //vector, .push_back()         -- storing coordinates
#include <sstream>  //stringstream, .str()         -- converting int to string
#include <cmath>    //log2()                       -- for color setting

using namespace std;

//exported prototypes
extern string int_to_string (int);

//internal prototypes
static string format        (int);
static string get_color_for (int);

//internal constants
static const int    BUFFER_SIZE_X    = 2;    //Spaces between tiles
static const int    BUFFER_SIZE_Y    = 2;    //Newlines between lines of tiles
static const int    BOX_LENGTH       = 5;    //width of each tile
static const string COLOR_CODES[14]  =       //the ANSI codes for colors
{"41;1",    "42;1",    "43;1",    "44;1",    "45;1",    "46;1",    "47;1",
 "47;1;30", "47;1;31", "47;1;32", "47;1;33", "47;1;34", "47;1;35", "47;1;36"};
//The 40's control background colors, 30s control text colors

Grid::Grid()    //Constructor, the grid is initially a NULL pointer
{
  score = 0;
  grid  = NULL;
}

Grid::~Grid()   //Destructor, calls delete_grid() to deallocate the grid
{
  delete_grid();
}

void Grid::create_grid(int size)  //Allocates and initializes grid
{
  grid_size = size;
  grid = new int*[grid_size];            //Allocate grid to hold <size> pointers
  for (int i = 0; i < grid_size; i++)    //Set each pointer to point to
    grid[i] = new int[grid_size];        //a <size> array of ints

  for (int i = 0; i < grid_size; i++)
    for (int j = 0; j < grid_size; j++)
      grid[j][i] = 0;                     //initialize each tile to 0
}

void Grid::delete_grid()    //Deallocates grid
{
  for (int i = 0; i < grid_size; i++)
    delete [] grid[i];      //Dereference each row

  delete [] grid;           //Dereference each collumn
  grid = NULL;
}

int Grid::get_score()     //Getter for the score of the round
{
  return score;
}

bool Grid::check_loss()   //Needed to end the game loop if you lose
{
  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++) {
      if (grid[j][i] == 0)    //If a tile is zero, you didn't lose
        return true;
      if (i+1 != grid_size && grid[j][i] == grid[j][i+1])
        return true;
      if (j+1 != grid_size && grid[j][i] == grid[j+1][i])
        return true;
      //If a tile below or to the right is the same value, you didn't lose
    }
  }
  return false;   //You only lost if none of these applied
}

void Grid::spawn(int tile_val = -1)   //Creates a tile at a random open spot
{
  static bool seeded = false; //Make sure that the rand function is seeded
  if (!seeded) {
    srand(time(NULL));
    seeded = true;
  }
  vector<pair <int, int> > open_indicies; //Make a vector of indexes
  int num_open = 0, num_to_spawn;
  if (tile_val == -1)
    num_to_spawn = (rand() % 10 == 0) ? 4 : 2; //10% = 4, 90% = 2
  else
    num_to_spawn = tile_val;    //Only make the value random if it isn't given
  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++) {
      if (grid[j][i] == 0) {
        open_indicies.push_back(make_pair(j, i)); //Add free indicies to vector
        num_open++;
      }
    }
  }
  if (num_open == 0)
    return;
      //Function just returns if you cannot spawn another tile
  int coord = rand() % num_open;
  grid
    [open_indicies[coord].first]                  //y coord
    [open_indicies[coord].second] = num_to_spawn; //x coord
}

extern string int_to_string(int i) //Gives functionality of the C++11 function
{
  stringstream s;     //Creates a stream...
  s << i;             //...pushes the integer into it...
  return s.str();     //...and then takes the integer back out as a string.
}

static string get_color_for(int val)
{
  int color_num;
  if (val < 0)
    color_num = 0;            //turns the 0 into a 0
  else
    color_num = log2(val);    //Turns the 2-4-8 into 1-2-3, easier to index
  if (color_num > 13)
    color_num = 13;           //the table maxes out at 2^13, so don't segfault

  return COLOR_CODES[color_num];
}

static string format(int num = -1) //formats the value so it can be printed
{
  string color_code = get_color_for(num);
  if (num == -1) {
    string spaces1 ((BOX_LENGTH - 1)/2, ' ');       //Rounds down
    string spaces2 (1 + (BOX_LENGTH - 2) / 2, ' '); //Rounds up
    return ("\033[" + color_code + "m" + spaces1 + "x" + spaces2 + "\033[0m");
  }
  string str = int_to_string(num);
  string spaces1 ((BOX_LENGTH - str.length()) / 2, ' ');
    //This division rounds down, and spaces1 is defined as ' '*<that num>
  string spaces2 (1 + (BOX_LENGTH - 1 - str.length()) / 2, ' '); //Rounds up
  str = "\033[" + color_code + "m" + spaces1 + str + spaces2 + "\033[0m";
  return str;
}

void Grid::show()
{
  cout << "\033[2J" << endl;            //the "clear screen" escape code

  cout << "Current score: " << score << "\n\n" << endl; // your score

  for (int j = 0; j < grid_size; j++) {       // y coord
    for (int i = 0; i < grid_size; i++) {     // x coord
      string val = (grid[j][i] == 0) ? format() : format(grid[j][i]);
        //Convert zeros into "x"s, but keep the other values the same
      cout << val << string (BUFFER_SIZE_X, ' ');
    }
    cout << string (BUFFER_SIZE_Y, '\n');
  }
}

bool Grid::combine(int x1, int y1, int x2, int y2) //(x1, y1) -> (x2, y2)
{
  int * tile1 = &grid[y1][x1];    //need to use pointer so
  int * tile2 = &grid[y2][x2];    //we can change the real value
  bool combined = false;

  if (*tile1 == *tile2 && *tile1 != 0) {  //Combines the tiles, score increase
    *tile2 *= 2;
    *tile1 = 0;
    score += *tile2;
    combined = true;
  }
  return combined;
}

bool Grid::slide(int x1, int y1, int x2, int y2)
{
  int * tile1 = &grid[y1][x1];
  int * tile2 = &grid[y2][x2];
  bool moved = false;

  if (*tile2 == 0 && *tile1 != 0) {    //Just moves the tile, no score increase
    *tile2 = *tile1;
    *tile1 = 0;
    moved = true;
  }
  return moved;
}

bool Grid::move(char movement)
{
  switch (movement) {
    case 'a':
      left();
      return check_loss();
    case 'w':
      up();
      return check_loss();
    case 'd':
      right();
      return check_loss();
    case 's':
      down();
      return check_loss();
    default:
      cout << "You should really fix your input validation..." << endl;
      return true;
  }
}

//Because of the many loops in my movement functions,
//I created a macro for each one which uses the i variable
//for x values and j variable for y values. Hope it is more
//readable this way.
#define downloop(expression) \
for (int i = 0; i < grid_size; i++) { \
  for (int j = grid_size - 1; j > 0; j--) { \
    expression \
  } \
}

void Grid::down()
{
  bool moved = false;   //You don't spawn any new tiles if nothing was moved
  for (int k = 0; k < grid_size; k++)
    downloop(if (slide(i, j-1, i, j)) moved = true;)
    //Ensures that the tiles are pushed to the right as far as they can

  downloop(if (combine(i, j-1, i, j)) moved = true;)
  //Actually combines them

  for (int k = 0; k < grid_size; k++)
    downloop(if (slide(i, j-1, i, j)) moved = true;)
    //Ensures that the tiles are again pushed right as much as possible
  if (moved)
    spawn();
}

//The same comment from the downloop works for the following functions too
#define uploop(expression) \
for (int i = 0; i < grid_size; i++) { \
  for (int j = 0; j < grid_size - 1; j++) { \
    expression \
  } \
}

void Grid::up()
{
  bool moved = false;
  for (int k = 0; k < grid_size; k++)
    uploop(if (slide(i, j+1, i, j)) moved = true;)
    //Ensures that the tiles are pushed to the right as far as they can

  uploop(if (combine(i, j+1, i, j)) moved = true;)
  //Actually combines them

  for (int k = 0; k < grid_size; k++)
    uploop(if (slide(i, j+1, i, j)) moved = true;)
    //Ensures that the tiles are again pushed right as much as possible
  if (moved)
    spawn();
}

#define leftloop(expression) \
for (int i = 0; i < grid_size - 1; i++) { \
  for (int j = 0; j < grid_size; j++) { \
    expression \
  } \
}

void Grid::left()
{
  bool moved = false;
  for (int k = 0; k < grid_size; k++)
    leftloop(if (slide(i+1, j, i, j)) moved = true;)
    //Ensures that the tiles are pushed to the right as far as they can

  leftloop(if (combine(i+1, j, i, j)) moved = true;)
  //Actually combines them

  for (int k = 0; k < grid_size; k++)
    leftloop(if (slide(i+1, j, i, j)) moved = true;)
    //Ensures that the tiles are again pushed right as much as possible
  if (moved)
    spawn();
}

#define rightloop(expression) \
for (int i = grid_size - 1; i > 0; i--) { \
  for (int j = 0; j < grid_size; j++) { \
    expression \
  } \
}

void Grid::right()
{
  bool moved = false;
  for (int k = 0; k < grid_size; k++)
    rightloop(if (slide(i-1, j, i, j)) moved = true;)
    //Ensures that the tiles are pushed to the right as far as they can

  rightloop(if (combine(i-1, j, i, j)) moved = true;)
  //Actually combines them

  for (int k = 0; k < grid_size; k++)
    rightloop(if (slide(i-1, j, i, j)) moved = true;)
    //Ensures that the tiles are again pushed right as much as possible
  if (moved)
    spawn();
}

#endif
