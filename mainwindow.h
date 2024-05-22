// In this file, we define our window properties.
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "board_ui.h"
#include "server.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setServer(Server* _server) {
        server = _server;
        serverBoard->connectWith(server);
    }

private:
    Ui::MainWindow *ui;
    board_ui *serverBoard;
    Server* server;
};

#endif // MAINWINDOW_H
