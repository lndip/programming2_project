#include "numbertile.hh"
#include "gameboard.hh"
#include <iomanip>
#include <iostream>

Coords operator+(Coords lhs, Coords rhs)
{
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

NumberTile::NumberTile(int value, Coords coords, GameBoard* board):
    value_(value), coords_(coords), board_(board), is_merged_(false), is_blocked_(false)
{
}

NumberTile::~NumberTile()
{
}

void NumberTile::print(int width)
{
    std::cout << "|" << std::setw(width - 1) << value_;
}

bool NumberTile::move(Coords direction, const int goal_val)
{
    Coords curr_loc = coords_;
    Coords new_loc = coords_ + direction;
    while( is_on_board( new_loc ) )
    {
        NumberTile* curr = board_->get_item(curr_loc);
        NumberTile* dest = board_->get_item(new_loc);

        if(curr->is_blocked_ == true or dest->is_blocked_ == true){
            // Breaks the loop if the current block is blocked or
            // encountering a blocked tile
            break;
        } else{
            if( dest->value_ == 0 )
            {
                dest->value_ = curr->value_;
                dest->is_merged_ = curr->is_merged_;
                curr->value_ = 0;
                curr->is_merged_ = false;
            }
            else if( dest->value_ == curr->value_ and
                     not dest->is_merged_ and
                     not curr->is_merged_ )
            {
                dest->value_ = 2 * curr->value_;
                curr->value_ = 0;
                dest->is_merged_ = true;
                return dest->value_ == goal_val;
            }
            curr_loc = new_loc;
            new_loc = dest->coords_ + direction;
        }
    }
    return false;
}

bool NumberTile::new_value(int new_val)
{
    if( value_ == 0 and not is_blocked_)
    {
        value_ = new_val;
        return true;
    }
    return false;
}

bool NumberTile::is_empty()
{
    return value_ == 0;
}

void NumberTile::reset_turn()
{
    is_merged_ = false;
}

int NumberTile::get_value()
{
    return value_;
}

void NumberTile::clear_value()
{
    value_ = 0;
}

bool NumberTile::is_merged()
{
    return is_merged_;
}

void NumberTile::block(bool block)
{
    if(block == true){
        is_blocked_ = true;
    } else {
        is_blocked_ = false;
    }
}

bool NumberTile::is_blocked()
{
    return is_blocked_;
}

bool NumberTile::is_on_board(Coords coords)
{
    return coords.first >= 0 and coords.first < board_->get_size() and
           coords.second >= 0 and coords.second < board_->get_size();
}
