/* Comp11
 * Homework 7
 * Skye Soss
 */

#ifndef __GAME_CPP__
#define __GAME_CPP__

//For debugging:
#define NLOG_GAME
#ifndef NLOG_GAME
#define L(x) do { clog << "* Log: " x << endl; } while (0)
#else
#define L(x) (void) 0
#endif

#include "grid.cpp" //Grid methods
#include "game.h"   //Game prototypes
#include <iostream> //cout, cin, <<, >>
#include <utility>  //pair, make_pair()
#include <fstream>  //ifstream, ofstream
#include <limits>   //max()
#include <vector>   //vector
#include <algorithm>//sort

using namespace std;

typedef pair<string, int> player_id; //Combination of name and score

//static prototypes
static void insert_to_file(player_id, string);
static bool player_comparison(player_id, player_id);
static string format(string s);
static string reformat(string s);

static const int STARTING_CT  = 2;
static const int STARTING_VAL = 2;
const bool GAME_MODE = true;
const bool TERMINAL_MODE = false;

Game::Game(string file)
{
  filename = file;
  game_over = false;
  for (int i = 0; i < 5; i++)
    high_scores[i] = make_pair("No_one", 0);
  L("Game constructed!");
}

Game::~Game()
{
  L("Game deconstructed!");
}

void Game::setup_grid(int size)
{
  my_grid.create_grid(size);
  for (int i = 0; i < STARTING_CT; i++)
    my_grid.spawn(STARTING_VAL);
  L("Grid of size "<<size<<" was setup and "<<STARTING_CT<<" tiles were spawned");
  my_grid.show();
}

void Game::turn(char move)
{
  switch (move) {
    case 'w':
    case 'a':
    case 's':
    case 'd':
      game_over = !my_grid.move(move);//Game is over if you cannot move
      my_grid.show();
      if (game_over)
        exit_game();
      break;
    case 'q':
      game_over = true;
      exit_game();
      break;
    case 'h':
      show_scores();
      break;
    default:
      cout << "Umm, you messed up the input validation" << endl;
  }
}

bool Game::is_over()
{
  return game_over;
}

extern void modify_buffer(bool mode)
{
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);//Get the current terminal I/O structure
  if (mode == GAME_MODE)
    t.c_lflag &= ~(ICANON | ECHO);       //Disable buffering and echo
  else
    t.c_lflag |= (ICANON | ECHO);        //Enable buffering and echo

  tcsetattr(STDIN_FILENO, TCSANOW, &t); //Apply the new settings now
}

void Game::exit_game()
{
  modify_buffer(TERMINAL_MODE);
  cout << "YOU LOST!!\n\n\n"
          "You can save your score to file with your name.\n"
          "Would you like to save your score of " << my_grid.get_score()
          << "? (y/n)"<< endl;
  char response;
  do
    cin >> response;
  while (response != 'y' && response != 'n');
  if (response == 'n') {
    cout << "Thanks for playing!" << endl;
    return;
  }
  string name;
  cout << "Enter your name: ";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
    //flush the newlines in stdin from cin calls
  getline(cin, name);
  player_id player_pair = make_pair(format(name), my_grid.get_score());
  insert_to_file(player_pair, filename);
  cout << "Thanks for playing!" << endl;
}

static bool player_comparison(player_id a, player_id b)
{     //This helper function allows the use of the sort function
      //on player_id vectors (used in insert_to_file())
  return a.second > b.second;
}

static void insert_to_file(player_id player, string filename)
{           //Function for adding players to the high-scores file
  vector<player_id> players;    //Creating the container to hold
  ifstream f_read;              //  players from the file
  f_read.open(filename.c_str());
  if (!f_read.is_open()) {
    L("" << filename << " does not exist, creating...");
    goto enter_scores;           //Skip the loading of previous players
  }                              //if the file doesn't exist
  while (!f_read.eof())
  {
    string name;
    f_read >> name;
    int score;
    f_read >> score;
    if (name.length() > 0) //Adding players from the file to the vector
      players.push_back(make_pair(name, score));
  }
  f_read.close(); //Closing the file (it was read only)
                  //Don't need to close if goto'd: was never open!
  enter_scores:

  players.push_back(player);    //Add our player_id
  sort(players.begin(), players.end(), &player_comparison); //easy sort
  ofstream f_write;       //open the file again, this time for writing
  f_write.open(filename.c_str());
  for (size_t i = 0; i < players.size(); i++)
    f_write << players[i].first << " " << players[i].second << endl;
  f_write.close();  //Add back the players, and close the write file
}

void Game::show_scores()
{
  static bool scores_loaded = false;
  if (!scores_loaded) {
    get_scores();
    L("Grabbed scores from "<<filename<<"!");
    scores_loaded = true;
  }
  my_grid.show();
  for (int i = 0; i < 5; i++) {
    cout << i+1 << ") " << reformat(high_scores[i].first)
         << "   \t" << high_scores[i].second << endl;
  }
  L("Showing scores!");
}

static string format(string s)
{
  replace( s.begin(), s.end(), ' ', '_');
  return s;
}

static string reformat(string s)
{
  replace( s.begin(), s.end(), '_', ' ');
  return s;
}

void Game::get_scores()
{
  ifstream file;
  file.open(filename.c_str());
  if (!file.good()) {
    for (int i = 0; i < 5; i++)
      high_scores[i] = make_pair("No_one", 0);
    return;
  }
  if (!file.is_open())
    cout << "Cannot open the file!!" << endl;
  string name;
  int score;
  for (int i = 0; i < 5; i++) {
    file >> name;
    if (file.eof())
      break;
    file >> score;
    high_scores[i] = make_pair(name, score);
  }
}

#ifndef __2048_CPP__
int main()
{

  Game my_game("scores.txt");
  int size;
  cout << "Enter size" << endl;
  cin >> size;
  my_game.setup_grid(size);
  modify_buffer(GAME_MODE);
  char move;
  while (!my_game.is_over()) {
    cin >> move;
    my_game.turn(move);
  }
  return 0;
}
#endif

#endif
