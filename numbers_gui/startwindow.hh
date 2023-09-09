/* Class Startwindow
 * ----------
 * COMP.CS.110 SPRING 2022
 * ----------
 * Class for the start window of the game 2048
 *
 * The class is based on QDialog, It gets the information of the game setting
 * for the users. The information here includes board size, seed for randoming,
 * goal for the game, coordinates of the blocked tile (if there is), and the
 * value m (after m moves, a random tile will be removed from board i.e set
 * its value to 0).
 *
 * Detailed explanation will be included in the documentation.
 *
 * Student: Diep Luong
 * Student number:
 * Email: diep.luong@tuni.fi
 * */
#ifndef STARTWINDOW_HH
#define STARTWINDOW_HH

#include <QDialog>

using Coords = std::pair<int, int>;

namespace Ui {
class StartWindow;
}

class StartWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

    // Getters
    int get_seed_value();
    int get_goal_value();
    int get_board_size();
    int get_m();
    Coords get_block_cor();

public: signals:
    void startWindowFinished(StartWindow* self);

private:
    Ui::StartWindow *ui;

    // Check if the goal is the power of 2
    bool isPowerOf2(int& number);

    // Check if the input is an integer
    bool isInteger(std::string input);

    // Check the input and assign value to the class attributes
    bool read_board_size();
    bool read_seed();
    bool read_goal();
    bool read_m();
    bool read_blockinf();

    // Emit signal if reading input succeeds
    void run_startwindow();

    int seed_;
    int goal_;
    int board_size_;
    int m_;
    int block_x_;
    int block_y_;

    // Constants
    const int DEFAULT_GOAL = 2048;
    const QString ERROR_GOAL2 = "The goal value must be a power of 2.";
    const QString ERROR_GOALMAX = "The maximum value of the goal corresponding to this board size is ";
    const QString EXCEED_BOARD = "The coordinate(s) not in the range of the board.";
};

#endif // STARTWINDOW_HH
