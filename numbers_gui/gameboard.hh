/* Class Gameboard
 * ----------
 * COMP.CS.110 SPRING 2022
 * ----------
 * Class for the gameboard of the game 2048, which is provided to the students
 *
 * The class manages the operations regarding to the gameboard of the game,
 * including creating the board, keeping track of the moving tiles and the scores
 *
 * Necessary modifications have been applied to the class. Detailed explanation
 * will be included in the documentation.
 *
 * Student: Diep Luong
 * Student number:
 * Email: diep.luong@tuni.fi
 * */
#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include "numbertile.hh"
#include <vector>
#include <random>

const int PRINT_WIDTH = 5;
const int NEW_VALUE = 2;
const int DEFAULT_GOAL = 2048;

class GameBoard
{
public:
    // Constructor
    GameBoard();

    // Destructor
    ~GameBoard();

    // Initializes the gameboard with nullptrs if the board size changes
    void init_empty();

    // Initializes the random number generator and fills the gameboard
    // with random numbers.
    // Blocks a tile if the user choose to have one blocked.
    void fill(int seed, Coords& bcoords);

    // Draws a new location (coordinates) from the random number generator and
    // puts the NEW_VALUE on that location, unless check_if_empty is true and
    // the gameboard is full.
    void new_value(bool check_if_empty = true);

    // Returns true, if all the tiles in the game board are occupied,
    // otherwise returns false.
    bool is_full() const;

    // Prints the game board.
    void print() const;

    // Moves the number tiles in the gameboard, if possible (by calling
    // move method for each number tile).
    // Finally, resets turn of all number tiles.
    bool move(Coords dir, int goal);

    // Returns the element (number tile) in the given coordinates.
    NumberTile* get_item(Coords coords);

    // Sets the size of the board
    void set_size(int& size);

    // Gets size of the board
    int get_size();

    // Gets score
    int get_score();

    // Sets m_, a tile is randomly removed after m_ moves
    void set_m(int& m);

    // A public attribute that tells if the board has a blocked tile
    bool has_block = false;

private:
    // Internal structure of the game board
    std::vector<std::vector<NumberTile*>> board_;

    // Random number generator and distribution,
    // they work better, if they are attributes of a class.
    std::default_random_engine randomEng_;
    std::uniform_int_distribution<int> distribution_;

    // The attributes of the class
    int size_;
    int score_;

    int moves_;
    int m_;
};

#endif // GAMEBOARD_HH
