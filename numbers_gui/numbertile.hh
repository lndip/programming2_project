/* Class Numbertile
 * ----------
 * COMP.CS.110 SPRING 2022
 * ----------
 * Class for the number tile of the game 2048, which is provided to the students
 *
 * The class manages the operations regarding to the tiles of the game,
 * including keeping track of its movement, setting the value to it.
 *
 * Necessary modifications have been applied to the class. Detailed explanation
 * will be included in the documentation.
 *
 * Student: Diep Luong
 * Student number:
 * Email: diep.luong@tuni.fi
 * */
#ifndef NUMBERTILE_HH
#define NUMBERTILE_HH

#include <vector>

using Coords = std::pair<int, int>;

// Forward declaration, to avoid circular include's
class GameBoard;

class NumberTile
{
public:
    // Constructor
    NumberTile(int value, Coords coords, GameBoard* board);

    // Destructor
    ~NumberTile();

    // Prints the number tile.
    void print(int width);

    // Moves the number tile in the given direction and merges it, if possible.
    // Returns true, if the player won the game, i.e. if the goal value was
    // reached, otherwise returns false.
    bool move(Coords direction, int goal_val);

    // Sets a new value for an empty number tile.
    // Returns true, if a new value was set, otherwise returns false.
    bool new_value(int new_val);

    // Returns true, if the number tile is empty, i.e. if it has the value 0.
    bool is_empty();

    // Sets the value of is_merged_ as false.
    void reset_turn();

    // Get the value of the number tile
    int get_value();

    // Reset the value of the number tile to 0
    void clear_value();

    // Check if a tile is merged
    bool is_merged();

    // Block a tile
    void block(bool block);

    // Check if a tile is blocked
    bool is_blocked();

private:
    // Value in the number tile
    int value_;

    // Coordinates of the number tile
    Coords coords_;

    // Pointer to the gameboard, where the number tile lies
    GameBoard* board_;

    // Tells if the number is merged or not,
    // can be true only temporarily during moves
    bool is_merged_;

    // Tells if a tile is blocked or not
    bool is_blocked_;

    // Returns true, if the given coordinates are on the gameboard,
    // otherwise returns false.
    bool is_on_board(Coords coords);
};

#endif // NUMBERTILE_HH
