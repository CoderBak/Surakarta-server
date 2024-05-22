#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        serverBoard(new board_ui(this)) {
    ui->setupUi(this);
    auto *layout = new QVBoxLayout;
    layout->addWidget(serverBoard);
    centralWidget()->setLayout(layout);
    centralWidget()->setFixedSize(sub_WIDTH, sub_HEIGHT);
    this->setFixedSize(sub_WIDTH, sub_HEIGHT);
}

MainWindow::~MainWindow() {
    delete ui;
}
