/*
 * Copyright 2019 CALIBRATION
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{//initial function
    ui->stackedWidget->setCurrentIndex(0);
//    ui->statusbar->showMessage(pageName.at(0));
}

void MainWindow::on_actionHome_triggered()
{// Home Page
    ui->stackedWidget->setCurrentIndex(0);
//    ui->statusbar->showMessage(pageName.at(0));
}

void MainWindow::on_actioncalN_triggered()
{//reflect Page
    ui->stackedWidget->setCurrentIndex(1);
//    ui->statusbar->showMessage(pageName.at(1));
}

void MainWindow::on_actioncalAngle_triggered()
{// Angle Page
    ui->stackedWidget->setCurrentIndex(2);
//    ui->statusbar->showMessage(pageName.at(2));
}

void MainWindow::on_actionBRDF_triggered()
{// BRDF Page
    ui->stackedWidget->setCurrentIndex(3);
//    ui->statusbar->showMessage(pageName.at(3));
}
// Home Button
void MainWindow::on_btnHomeCalN_clicked()
{
    on_actioncalN_triggered();
}

void MainWindow::on_btnHomeCalAngle_clicked()
{
    on_actioncalAngle_triggered();
}

void MainWindow::on_btnHomeBRDF_clicked()
{
    on_actionBRDF_triggered();
}

void MainWindow::on_actionExit_triggered()
{//Exit
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{// Open Local Dir

}

void MainWindow::on_btnCalNCal_clicked()
{//Cal for signle and range

}
