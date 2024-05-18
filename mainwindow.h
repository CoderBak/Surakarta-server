#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QMessageBox>
#include "NetworkLibrary/networkdata.h"
#include "NetworkLibrary/networkserver.h"
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
    int port = 1234;
    const int max_clients = 2;
    Ui::MainWindow *ui;
    QSet<QTcpSocket *> clients;
    NetworkServer *server = nullptr;
    QTcpSocket *client1 = nullptr;
    QTcpSocket *client2 = nullptr;
    board_ui *serverBoard;
    Server* server2;

    void send_to_another_client(QTcpSocket *another, NetworkData data);

    void remove_client(QTcpSocket *client);

private slots:

    void listen_port();

    void restart_server();

    void receive_from_client(QTcpSocket *client, NetworkData data);
};

#endif // MAINWINDOW_H
