#include "startwindow.hh"
#include "ui_startwindow.h"

#include <bits/stdc++.h>
#include <cmath>

StartWindow::StartWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    // Name the window
    QDialog::setWindowTitle("Game information");
    ui->goalSpinBox->setMaximum(1999999999);
    ui->goalSpinBox->setMinimum(4);
    ui->goalSpinBox->setValue(DEFAULT_GOAL);
    ui->sizeSpinBox->setMinimum(4);

    connect(ui->finishPushButton, &QPushButton::clicked, this, &StartWindow::run_startwindow);
}

StartWindow::~StartWindow()
{
    delete ui;
}

int StartWindow::get_seed_value()
{
    return seed_;
}

int StartWindow::get_goal_value()
{
    return goal_;
}

int StartWindow::get_board_size()
{
    return board_size_;
}

int StartWindow::get_m()
{
    return m_;
}

Coords StartWindow::get_block_cor()
{
    Coords coords = std::make_pair(block_y_, block_x_);
    return coords;
}

bool StartWindow::isPowerOf2(int &number)
{
    if(number == 0){
        return false;
    }
    return std::ceil(std::log2(number)) == std::floor(std::log2(number));
}

bool StartWindow::read_board_size()
{
    board_size_ =ui->sizeSpinBox->value();
    return true;
}

bool StartWindow::read_seed()
{
    seed_ = ui->seedSpinBox->value();
    return true;
}

bool StartWindow::read_goal()
{
    goal_ = ui->goalSpinBox->value();
    if(not isPowerOf2(goal_)){
        ui->errorTextBrowser->setText(ERROR_GOAL2);
        return false;
    } else if(goal_ > std::pow(2, board_size_*board_size_)){
        ui->errorTextBrowser->setText(ERROR_GOALMAX +
                                      QString::number(long(std::pow(2, board_size_*board_size_))));
        return false;
    }
    return true;
}

bool StartWindow::read_m()
{
    m_ = ui->mSpinBox->value();
    return true;
}

bool StartWindow::read_blockinf()
{
    if(ui->blockRadioButton->isChecked()){
        block_x_ = ui->xblockSpinBox->value();
        block_y_ = ui->yblockSpinBox->value();
        if(block_x_ > board_size_-1 or block_y_ > board_size_-1){
            ui->errorTextBrowser->setText(EXCEED_BOARD);
            return false;
        }
    } else{
        block_x_ = -1;
        block_y_ = -1;
    }
    return true;
}

void StartWindow::run_startwindow()
{
    if(read_board_size() and read_seed() and read_goal() and read_m() and read_blockinf()){
        emit startWindowFinished(this);
    }
}




