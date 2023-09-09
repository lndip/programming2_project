#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QKeyEvent>
#include <iostream>

#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Name the window
    this->setWindowTitle("2048 game");

    timer_ = new QTimer(this);
    start_window_ = new StartWindow(this);
    start_window_->show();

    // Set the display of the widgets
    ui->statusTextBrowser->setStyleSheet("background:transparent");
    ui->graphicsView->setStyleSheet("background:transparent");
    ui->scoreLabel->setAlignment(Qt::AlignCenter);
    ui->bscoreLabel->setAlignment(Qt::AlignCenter);

    QPixmap right(PREFIX + "right" + SUFFIX);
    ui->rightPushButton->setIcon(right);
    QPixmap left(PREFIX + "left" + SUFFIX);
    ui->leftPushButton->setIcon(left);
    QPixmap up(PREFIX + "up" + SUFFIX);
    ui->upPushButton->setIcon(up);
    QPixmap down(PREFIX + "down" + SUFFIX);
    ui->downPushButton->setIcon(down);

    // When the start window closes, the given information is stored
    connect(start_window_, &StartWindow::startWindowFinished, this, &MainWindow::finishStartWindow);
    // After getting the information, initilizes the board
    connect(this, &MainWindow::readInforamtionDone, this, &MainWindow::initilizeBoard);
    // Creates graphic board afterwards
    connect(this, &MainWindow::initializeBoardDone, this, &MainWindow::createGraphicBoard);
    // Updates the graphic board according to the created graphic board, the game is ready for users
    connect(this, &MainWindow::createGraphicBoardDone, this, &MainWindow::updateGraphicBoard);

    // Moves the tiles according to the chosen directions
    connect(ui->upPushButton, &QPushButton::clicked, this, &MainWindow::move);
    connect(ui->downPushButton, &QPushButton::clicked, this, &MainWindow::move);
    connect(ui->leftPushButton, &QPushButton::clicked, this, &MainWindow::move);
    connect(ui->rightPushButton, &QPushButton::clicked, this, &MainWindow::move);
    // Updates the graphic board after a move
    connect(this, &MainWindow::moveDone, this, &MainWindow::updateGraphicBoard);

    // Restarts the game on clicking the Restart button
    connect(ui->restartPushButton, &QPushButton::clicked, this, &MainWindow::restart);

    // Shows the time after the timeout signal of the timer
    connect(timer_, &QTimer::timeout, this, &MainWindow::show_time);

    // Pauses and contimues the game on Pause and Continue button clicks
    connect(ui->pausePushButton, &QPushButton::clicked, this, &MainWindow::pauseGame);
    connect(ui->conPushButton, &QPushButton::clicked, this, &MainWindow::inGame);

    // Quits the game and closes the window on Quit button click
    connect(ui->quitPushButton, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(timeRuns_){
        if(event->key() == Qt::Key_W or event->key() == Qt::Key_Up){
            emit ui->upPushButton->clicked();
        } else if(event->key() == Qt::Key_S or event->key() == Qt::Key_Down){
            emit ui->downPushButton->clicked();
        } else if(event->key() == Qt::Key_A or event->key() == Qt::Key_Left){
            emit ui->leftPushButton->clicked();
        } else if(event->key() == Qt::Key_D or event->key() == Qt::Key_Right){
            emit ui->rightPushButton->clicked();
        }
    }
}

void MainWindow::finishStartWindow(StartWindow *start_window)
{
    // Gets the given information
    seed_ = start_window->get_seed_value();
    goal_ = start_window->get_goal_value();
    board_size_ = start_window->get_board_size();
    m_ = start_window->get_m();
    block_coords_ = start_window->get_block_cor();
    // Sets the time to 0:0
    minutes_ = 0;
    seconds_ = 0;

    start_window->hide();
    this->show();

    emit readInforamtionDone();
}

void MainWindow::initilizeBoard()
{
    board_.set_size(board_size_);
    board_.set_m(m_);
    board_.init_empty();
    if(block_coords_ != std::make_pair(-1,-1)){
        board_.has_block = true;
    } else{
        board_.has_block = false;
    }
    board_.fill(seed_, block_coords_);

    ui->secLcdNumber->display(seconds_);
    ui->minLcdNumber->display(minutes_);
    ui->minLcdNumber->setStyleSheet("");
    ui->secLcdNumber->setStyleSheet("");

    inGame();

    emit initializeBoardDone();
}

void MainWindow::createGraphicBoard()
{
    scene_ = new QGraphicsScene(this);

    // Calculates the sizes for the grahic board
    square_size_ = SIZE/board_size_;
    ui->graphicsView->setGeometry(50, 160, SIZE*1.05, SIZE*1.05);
    ui->graphicsView->setScene(scene_);

    scene_->setSceneRect(0, 0, SIZE, SIZE);

    s_labels_ = {};

    for(int y = 0; y < board_size_; y++){
        std::vector<QLabel*> row_labels;
        for(int x = 0; x < board_size_; x++){
            QBrush blackBrush(Qt::black);
            QPen blackPen(Qt::black);
            // Add graphic rectangles
            QGraphicsRectItem* rec = scene_->addRect(x*square_size_, y*square_size_,
                                                     square_size_, square_size_, blackPen, blackBrush);
            // Add a Label to display number in each graphic rectangle
            QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(rec);
            QLabel* sLabel = new QLabel();
            sLabel->resize(square_size_*0.95, square_size_*0.95);
            sLabel->setAlignment(Qt::AlignCenter);
            proxy->setWidget(sLabel);
            proxy->setPos(rec->boundingRect().center()-sLabel->rect().center());
            row_labels.push_back(sLabel);
        }
       s_labels_.push_back(row_labels);
    }
    emit createGraphicBoardDone();
}

void MainWindow::updateGraphicBoard()
{
    for(int y = 0; y < board_size_; y++){
        for(int x = 0; x < board_size_; x++){

            Coords coords = std::pair(y,x);
            NumberTile* num = board_.get_item(coords);
            if(coords == block_coords_){
                // Set the background color of the blocked tile to black
                s_labels_.at(block_coords_.first).at(block_coords_.second)
                        ->setStyleSheet("background:black");
            } else {
                if(num->get_value() != 0){
                    int exponent = std::log2(num->get_value());
                    if(exponent <= 16){
                        // Set the background color and the image for the labels
                        s_labels_.at(y).at(x)->setStyleSheet("background:" + COLORS.at(exponent-1));

                        QPixmap image(PREFIX + QString::number(exponent) + SUFFIX);
                        s_labels_.at(y).at(x)->setPixmap(image.scaled(square_size_*0.95, square_size_*0.95));
                    } else{
                        // Set the background color for number exceeding 2^16
                        // If the number exceeds 2^16 then the label display the number as text
                        s_labels_.at(y).at(x)->setStyleSheet("background:" + COLORS.at(16));
                        s_labels_.at(y).at(x)->setText(QString::number(num->get_value()));
                    }
                } else{
                    // Set display for empty tile
                    s_labels_.at(y).at(x)->setText("");
                    s_labels_.at(y).at(x)->setStyleSheet("");
                    QPixmap blank_image("");
                    s_labels_.at(y).at(x)->setPixmap(blank_image);
                }
            }
        }
    }
    // Set the text for displaying the scores
    ui->scoreTextBrowser->setText(QString :: number (board_.get_score()));
    ui->bscoreTextBrowser->setText(QString::number(max_score(scores_)));
}

void MainWindow::move()
{
    Coords dir;
    if(sender() == ui->upPushButton){
        dir = UP;
    } else if(sender() == ui->downPushButton){
        dir = DOWN;
    } else if(sender() == ui->leftPushButton){
        dir = LEFT;
    } else if(sender() == ui->rightPushButton){
        dir = RIGHT;
    }

    if(board_.move(dir, goal_)){
        pauseGame();
        scores_.push_back(board_.get_score());
        // Set the display when the winner wins
        ui->statusTextBrowser->setText(WIN);
        ui->minLcdNumber->setStyleSheet("background: rgb(118, 186, 27)");
        ui->secLcdNumber->setStyleSheet("background: rgb(118, 186, 27)");
        ui->conPushButton->setEnabled(false);
        ui->pausePushButton->setEnabled(false);

    } else if(board_.is_full()){
        pauseGame();
        scores_.push_back(board_.get_score());
        // Set the display when the winner loses
        ui->statusTextBrowser->setText(LOSE);
        ui->minLcdNumber->setStyleSheet("background: rgb(255, 82, 82)");
        ui->secLcdNumber->setStyleSheet("background: rgb(255, 82, 82)");
        ui->conPushButton->setEnabled(false);
        ui->pausePushButton->setEnabled(false);

    } else{
        board_.new_value(false);
    }
//    board_.print();
    emit moveDone();
}

void MainWindow::restart()
{
    scores_.push_back(board_.get_score());
    this->hide();
    start_window_->show();
}

void MainWindow::show_time()
{
    calculate_time();
    ui->minLcdNumber->display(minutes_);
    ui->secLcdNumber->display(seconds_);
}

void MainWindow::pauseGame()
{
    timeRuns_ = false;
    ui->statusTextBrowser->setText(PAUSE_GAME);
    stop_timer();
    // Disable the push buttons
    ui->leftPushButton->setEnabled(false);
    ui->rightPushButton->setEnabled(false);
    ui->upPushButton->setEnabled(false);
    ui->downPushButton->setEnabled(false);
}

void MainWindow::inGame()
{
    timeRuns_ = true;
    ui->statusTextBrowser->setText(IN_GAME);
    start_timer();
    // Enable the push buttons
    ui->leftPushButton->setEnabled(true);
    ui->rightPushButton->setEnabled(true);
    ui->upPushButton->setEnabled(true);
    ui->downPushButton->setEnabled(true);
}

void MainWindow::calculate_time()
{
    seconds_ += 1;
    if(seconds_ == 60){
        seconds_ = 0;
        minutes_ += 1;
    }
}

void MainWindow::start_timer()
{
    if(not timer_->isActive()){
        timer_->start(1000);
    }
}

void MainWindow::stop_timer()
{
    if(timer_->isActive()){
        timer_->stop();
    }
}

int MainWindow::max_score(std::vector<int> &scores)
{
    int max = 0;
    if(scores.size() != 0){
        for(int score : scores){
            if(score >= max){
                max = score;
            }
        }
    }
    return max;
}


