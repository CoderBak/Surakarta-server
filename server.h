#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>

class Server : public QTcpServer {
Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void startGame();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void socketDisconnected1();
    void socketDisconnected2();
    void socketDisconnected();

private:
    QTcpSocket *client1, *client2;
};

#endif // SERVER_H
