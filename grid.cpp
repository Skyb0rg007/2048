/* Comp11
 * Homework 7
 * Skye Soss
 */

#ifndef __GRID_CPP__
#define __GRID_CPP__

//For debugging:
#define NLOG_GRID
#ifndef NLOG_GRID
#define LOG(x) do { clog << "* Log: " x << endl; } while (0)
#else
#define LOG(x) (void) 0
#endif

#include "grid.h"   //Grid prototypes              -- Defined grid class
#include <string>   //string                       -- string modification
#include <iostream> //cout, clog, <<, >>, endl     -- standard I/O
#include <stdlib.h> //srand(), rand()              -- producing random numbers
#include <time.h>   //time()                       -- needed to seed srand()
#include <utility>  //pair, make_pair()            -- storing coordinates
#include <vector>   //vector, .push_back()         -- storing coordinates
#include <sstream>  //stringstream, .str()         -- converting int to string
#include <termios.h>//termios, tcgetattr(), tcsetattr(), .c_lflag()
#include <unistd.h> //STDIN_FILENO                 -- for buffer settings
#include <cmath>    //log2()                       -- for color setting

using namespace std;

//static prototypes
extern string int_to_string (int);
static string format        (int);
static string get_color_for (int);

static const int    BUFFER_SIZE_X    = 2;    //Spaces between tiles
static const int    BUFFER_SIZE_Y    = 2;    //Newlines between lines of tiles
static const int    BOX_LENGTH       = 5;    //width of each tile (height is always 1)
static const string COLOR_CODES[14] = {"41;1", "42;1", "43;1", "44;1", "45;1",
                                      "46;1", "47;1", "47;1;30", "47;1;31",
                                      "47;1;32", "47;1;33", "47;1;34",
                                      "47;1;35", "47;1;36"};


Grid::Grid()
{
  score = 0;
  grid  = NULL;
  LOG("Grid created!");
}

Grid::~Grid()
{
  delete_grid();
  LOG("Grid destroyed!");
}

void Grid::create_grid(int size)
{
  grid_size = size;
  grid = new int*[grid_size];            //Allocate grid to hold 'size' pointers
  for (int i = 0; i < grid_size; i++)    //Set each pointer to point to
    grid[i] = new int[grid_size];          //an 'n' size array of ints
}

void Grid::delete_grid()
{
  for (int i = 0; i < grid_size; i++)
    delete [] grid[i];
  delete [] grid;
  grid = NULL;
  LOG("2D array dereferenced");
}

int Grid::get_score()
{
  return score;
}

bool Grid::check_loss()
{
  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++) {
      if (grid[i][j] == 0)
        return true;
      if (j+1 != grid_size && grid[i][j] == grid[i][j+1])
        return true;
      if (i+1 != grid_size && grid[i][j] == grid[i+1][j])
        return true;
    }
  }
  return false;
}

void Grid::spawn(int tile_val = -1)
{
  static bool seeded = false; //Make sure that the rand function is seeded
  if (!seeded) {
    srand(time(NULL));
    seeded = true;
    // LOG("Srand seeded!");
  }
  vector<pair <int, int> > open_indicies; //Make a vector of indexes
  int num_open = 0, num_to_spawn;
  if (tile_val == -1)
    num_to_spawn = (rand() % 10 == 0) ? 4 : 2; //10% = 4, 90% = 2
  else
    num_to_spawn = tile_val;
  LOG("Spawned a " << num_to_spawn);
  for (int i = 0; i < grid_size; i++) {
    for (int j = 0; j < grid_size; j++) {
      if (grid[i][j] == 0) {
        open_indicies.push_back(make_pair(i, j)); //Add free indicies to vector
        num_open++;
      }
    }
  }
  // LOG("There are " << num_open << " open squares");
  if (num_open == 0)
    return;
      //Function returns if you cannot spawn another tile (you lose)
  int coord = rand() % num_open;
  grid
    [open_indicies[coord].first] //x coord
    [open_indicies[coord].second] = num_to_spawn; //y coord
}

extern string int_to_string(int i)
{
  stringstream s;
  s << i;
  return s.str();
}

static string get_color_for(int val)
{
  int color_num;
  if (val < 0)
    color_num = 0;
  else
    color_num = log2(val);
  if (color_num > 13)
    color_num = 13;

  return COLOR_CODES[color_num];
}

static string format(int num = -1) //static -> only used by show method
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
  for (int j = grid_size - 1; j >= 0; j--) {   // y coord (counts down)
    for (int i = 0; i < grid_size; i++) {      // x coord (counts up)
      string val = (grid[i][j] == 0) ? format() : format(grid[i][j]);
        //Convert zeros into "x"s, but keep the other values the same
      cout << val << string (BUFFER_SIZE_X, ' ');
    }
    cout << string (BUFFER_SIZE_Y, '\n');
  }
}

bool Grid::combine(int x1, int y1, int x2, int y2) //(x1, y1) -> (x2, y2)
{
  int * tile1 = &grid[x1][y1];
  int * tile2 = &grid[x2][y2];
  bool combined = false;
  // LOG("Combine1: (" << x1 << "," << y1 << ") = " << *tile1);
  // LOG("Combine2: (" << x2 << "," << y2 << ") = " << *tile2);
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
  int * tile1 = &grid[x1][y1];
  int * tile2 = &grid[x2][y2];
  bool moved = false;
  // LOG("Slide1: (" << x1 << "," << y1 << ") = " << *tile1);
  // LOG("Slide2: (" << x2 << "," << y2 << ") = " << *tile2);
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

#define uploop(expression) \
for (int i = 0; i < grid_size; i++) { \
  for (int j = grid_size - 1; j > 0; j--) { \
    expression \
  } \
}

void Grid::up()
{
  bool moved = false;
  for (int k = 0; k < grid_size; k++)
    uploop(if (slide(i, j-1, i, j)) moved = true;)
    //Ensures that the tiles are pushed to the right as far as they can

  uploop(if (combine(i, j-1, i, j)) moved = true;)
  //Actually combines them

  for (int k = 0; k < grid_size; k++)
    uploop(if (slide(i, j-1, i, j)) moved = true;)
    //Ensures that the tiles are again pushed right as much as possible
  if (moved)
    spawn();
}

#define downloop(expression) \
for (int i = 0; i < grid_size; i++) { \
  for (int j = 0; j < grid_size - 1; j++) { \
    expression \
  } \
}

void Grid::down()
{
  bool moved = false;
  for (int k = 0; k < grid_size; k++)
    downloop(if (slide(i, j+1, i, j)) moved = true;)
    //Ensures that the tiles are pushed to the right as far as they can

  downloop(if (combine(i, j+1, i, j)) moved = true;)
  //Actually combines them

  for (int k = 0; k < grid_size; k++)
    downloop(if (slide(i, j+1, i, j)) moved = true;)
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
