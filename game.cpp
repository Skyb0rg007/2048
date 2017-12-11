/* Comp11
 * Homework 7
 * Skye Soss
 */

#ifndef __GAME_CPP__
#define __GAME_CPP__

#include "grid.cpp" //Grid methods          -- To implement game methods
#include "game.h"   //Game prototypes       -- The prototypes we implement
#include "io.cpp"   //modify_buffer()       -- Changing the buffer settings
#include <iostream> //cout, cin, <<, >>     -- Basic I/O
#include <utility>  //pair, make_pair()     -- Keeps name and score together
#include <fstream>  //ifstream, ofstream    -- File I/O
#include <limits>   //max()                 -- See Grid::exit_game()
#include <vector>   //vector                -- See insert_to_file()
#include <algorithm>//sort                  -- See insert_to_file()

using namespace std;

//internal containers
typedef pair<string, int> player_id; //Combination of name and score

//exported prototypes
static void insert_to_file(player_id, string);
static bool player_comparison(player_id, player_id);
static string format(string s);
static string reformat(string s);

//internal constants
static const int STARTING_CT  = 2;
static const int STARTING_VAL = 2;

Game::Game(string file)   //Contructs the game, with the save-file specified
{
  filename = file;
  game_over = false;      //game isn't over yet!
  for (int i = 0; i < 5; i++)
    high_scores[i] = make_pair("No_one", 0); //high_scores starts with No_One's
}

Game::~Game() {}  //No need to decontruct anything--no dynamic allocation

void Game::setup_grid(int size) //initializes grid and spawns 2 2's
{
  my_grid.create_grid(size);
  for (int i = 0; i < STARTING_CT; i++) {
    my_grid.spawn(STARTING_VAL);
  }
  my_grid.show();
}

void Game::turn(char move) //The entire game, each possible move is represented
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

bool Game::is_over()    //getter for the game_over variable
{
  return game_over;
}

void Game::exit_game()    //What happens when you lose/quit
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
    //the first argument is the largest size cin can be
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
  if (!f_read.is_open())
    goto enter_scores;           //Skip the loading of previous players
                                 //if the file doesn't exist
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

void Game::show_scores()  //Displays the scores on the screen
{
  static bool scores_loaded = false;  //Only needs to load once
  if (!scores_loaded) {
    get_scores();
    scores_loaded = true;
  }
  my_grid.show();
  for (int i = 0; i < 5; i++) {
    cout << i+1 << ") " << reformat(high_scores[i].first)
         << "   \t" << high_scores[i].second << endl;
  }
}

static string format(string s)
{ //To make sure that we can save names with spaces, we replace them with
  //underscores in the file, and change back to spaces when displayed
  replace( s.begin(), s.end(), ' ', '_');
  return s;
}

static string reformat(string s)
{ //See comments for format()
  replace( s.begin(), s.end(), '_', ' ');
  return s;
}

void Game::get_scores()
{ //Loads the scores from the file
  ifstream file;
  file.open(filename.c_str());
  if (!file.good()) { //Keep the No_one's if can't load file
    for (int i = 0; i < 5; i++)
      high_scores[i] = make_pair("No_one", 0);
    return;
  }
  string name;
  int score;
  for (int i = 0; i < 5; i++) {
    file >> name;     //reads from the file, getting name and score
    if (file.eof())   //needs to read the eof before we can see it
                      //so this needs to be after the 'file>>name' line
      break;
    file >> score;
    high_scores[i] = make_pair(name, score);
  }
}

#endif
