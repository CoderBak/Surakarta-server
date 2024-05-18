#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QMessageBox>
#include <QLineEdit>
#include "board_ui.h"
#include "server.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    QLineEdit *PortEdit;
    QPushButton *PortButton;
    QLineEdit *ShowClient1;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *ShowClient2;
    QPushButton *restart;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void setServer(Server* _server) {
        server2 = _server;
        serverBoard->connectWith(server2);
    }

private:
    Ui::MainWindow *ui;
    QTcpSocket *client1 = nullptr;
    QTcpSocket *client2 = nullptr;
    board_ui *serverBoard;
    Server* server2;
};

#endif // MAINWINDOW_H
