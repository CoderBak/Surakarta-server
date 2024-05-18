#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        serverBoard(new board_ui(this)) {
    ui->setupUi(this);
    auto *layout = new QVBoxLayout;
    layout->addWidget(serverBoard);
    centralWidget()->setLayout(layout);
    centralWidget()->setFixedSize(_WIDTH, _HEIGHT);
    this->setFixedSize(_WIDTH, _HEIGHT);
}

MainWindow::~MainWindow() {
    delete ui;
}
