/* Class MainWindow
 * ----------
 * COMP.CS.110 SPRING 2022
 * ----------
 * Class for the main window of the game 2048
 *
 * The class manages the GUI of game. It controls the input information, operating
 * the game, calculating score, running the timer.
 *
 * Detailed explanation will be included in the documentation.
 *
 * Student: Diep Luong
 * Student number:
 * Email: diep.luong@tuni.fi
 * */
#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <startwindow.hh>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QTimer>
#include <vector>

#include "gameboard.hh"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Key press event that allow user to use the key
    // W, A, S, D to move up, left, down, and right.
    void keyPressEvent(QKeyEvent* event);

private: signals:
    void readInforamtionDone();

    void initializeBoardDone();

    void createGraphicBoardDone();

    void moveDone();

private slots:
    void finishStartWindow(StartWindow* start_window);

    void initilizeBoard();

    void createGraphicBoard();

    void updateGraphicBoard();

    void move();

    void restart();

    void show_time();

    // Pauses the game
    void pauseGame();
    // Starts / continues the game
    void inGame();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene_;
    GameBoard board_;
    // Start window to get the information of the game
    StartWindow* start_window_;

    // Moving directions in 2048
    const Coords DEFAULT_DIR = {0, 0};
    const Coords LEFT = {0, -1};
    const Coords UP = {-1, 0};
    const Coords RIGHT = {0, 1};
    const Coords DOWN = {1, 0};

    // The colors display for the tiles in the basic version
    // (board size = 4)
    const std::vector<QString> COLORS = {"rgb(221, 192, 180)",
                                        "rgb(209, 174, 155)",
                                        "rgb(192, 145, 117)",
                                        "rgb(218, 165, 32)",
                                        "rgb(255, 191, 0)",
                                        "rgb(253, 165, 15)",
                                        "rgb(235, 150, 5)",
                                        "rgb(247, 152, 98)",
                                        "rgb(249, 129, 42)",
                                        "rgb(239, 130, 13)",
                                        "rgb(239, 114, 21)",
                                        "rgb(240, 94, 35)",
                                        "rgb(215, 104, 44)",
                                        "rgb(179, 103, 43)",
                                        "rgb(190, 85, 4)",
                                        "rgb(129, 63, 11)",
                                        "rgb(136, 48, 0)"
                                        };

    // Game status
    const QString IN_GAME= "In game";
    const QString PAUSE_GAME = "Pause";
    const QString WIN = "Congratulation! You won";
    const QString LOSE = "Can't add new tile, you lost!";

    const QString PREFIX =":/images/";
    const QString SUFFIX = ".png";

    // Attributes relating to the size of the graphic board
    int square_size_;
    const int SIZE = 340;
    int board_size_;

    int seed_;
    int goal_;
    int m_;
    Coords block_coords_;

    // Data structure to store the labels corresponding to the tile
    std::vector<std::vector<QLabel*>> s_labels_;

    // Attributes and function for the time
    QTimer* timer_;
    int minutes_ = 0;
    int seconds_ = 0;
    // True if the timer is running
    bool timeRuns_ = true;
    // Calculates the time (minutes and seconds)
    void calculate_time();
    // Starts and stop the timer
    void start_timer();
    void stop_timer();

    //Atrribute and function for calculating scores
    // Scores' vector to store the score of every round
    std::vector<int> scores_;
    // Finds the maximum score for the vector
    int max_score(std::vector<int>& scores);
};
#endif // MAINWINDOW_HH
