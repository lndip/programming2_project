#include "gameboard.hh"
#include <iostream>

GameBoard::GameBoard()
{
}

GameBoard::~GameBoard()
{
    for(unsigned int y = 0; y < board_.size(); ++y)
    {
        for(unsigned int x = 0; x < board_.at(y).size(); ++x)
        {
            delete board_.at(x).at(y);
            board_.at(x).at(y) = nullptr;
        }
    }
}

void GameBoard::init_empty()
{
    // Initial conditions of the game
    has_block = false;
    moves_ = 0;
    score_ = 0;
    // Clears the board and reinitializes it when the board size change
    if(size_ != int(board_.size())){
        // Deallocates the dynamic variable before clearing the board
        for(auto y = 0; y < int(board_.size()); y++){
            for(auto x = 0; x < int(board_.size()); x++){
                delete board_.at(y).at(x);
            }
        }
        board_.clear();
        std::vector<NumberTile*> row;
        for( int i = 0; i < size_; ++i)
        {
            row.push_back(nullptr);
        }
        for( int i = 0; i < size_; ++i)
        {
            board_.push_back(row);
        }
    }
}

void GameBoard::fill(int seed, Coords& bcoords)
{
    randomEng_.seed(seed);
    distribution_ = std::uniform_int_distribution<int>(0, size_ - 1);

    // Wiping out the first random number (which is almost almost 0)
    distribution_(randomEng_);

    for( auto y = 0; y < size_; ++y )
    {
        for( auto x = 0; x < size_; ++x )
        {
            if(board_.at(y).at(x) == nullptr){
                board_.at(y).at(x) = new NumberTile(0, std::make_pair(y, x), this);
            } else{
                // Clears the value of the number tile if it exists
                //(in case the board size does not change)
                board_.at(y).at(x)->clear_value();
                board_.at(y).at(x)->block(false);
            }
        }
    }
    // Blocks a tile as the user's wish
    if(has_block){
        board_.at(bcoords.first).at(bcoords.second)->block(true);
    }

    for( int i = 0 ; i < size_ ; ++i )
    {
        new_value();
    }
}

void GameBoard::new_value(bool check_if_empty)
{
    if( check_if_empty and is_full() ){
        // So that we will not be stuck in a forever loop
        return;
    }
    int random_x = 0;
    int random_y = 0;
    do
    {
        random_x = distribution_(randomEng_);
        random_y = distribution_(randomEng_);
    } while( not board_.at(random_y).at(random_x)->new_value(NEW_VALUE) );
}

void GameBoard::print() const
{
    for( auto y : board_ )
    {
        std::cout << std::string(PRINT_WIDTH * size_ + 1, '-') << std::endl;
        for( auto x : y )
        {
            x->print(PRINT_WIDTH);
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::string(PRINT_WIDTH * size_ + 1, '-') << std::endl;
}

bool GameBoard::move(Coords dir, int goal)
{
    // Randomly remove a tile after m_ moves
    moves_ += 1;
    if(moves_ == m_){
        moves_ = 0;
        bool keep = true;
        while(keep){
            int rand_x = distribution_(randomEng_);
            int rand_y = distribution_(randomEng_);
            if(board_.at(rand_y).at(rand_x)->get_value() != 0){
                board_.at(rand_y).at(rand_x)->clear_value();
                keep = false;
            }
        }
    }

    bool has_won = false;
    for( unsigned y = 0; y < board_.size(); ++y )
    {
        for( unsigned x = 0; x < board_.front().size(); ++x )
        {
            int directed_y = dir.first > 0 ? board_.size() - y - 1 : y;
            int directed_x = dir.second > 0 ? board_.back().size() - x - 1 : x;
            if( board_.at(directed_y).at(directed_x)->move(dir, goal) )
            {
                has_won = true;
            }
        }
    }

    for( auto &row : board_ )
    {
        for( auto &tile : row )
        {
            if(tile->is_merged()){
                // Adding the score
                score_ += tile->get_value();
            }
            tile->reset_turn();
        }
    }
    return has_won;
}

NumberTile* GameBoard::get_item(Coords coords)
{
    return board_.at(coords.first).at(coords.second);
}

void GameBoard::set_size(int &size)
{
    size_ = size;
}

int GameBoard::get_size()
{
    return size_;
}

int GameBoard::get_score()
{
    return score_;
}

void GameBoard::set_m(int &m)
{
    m_ = m;
}

bool GameBoard::is_full() const
{
    for( auto y : board_ )
    {
        for( auto x : y )
        {
            if( x->is_empty() and not x->is_blocked())
            {
                return false;
            }
        }
    }
    return true;
}
