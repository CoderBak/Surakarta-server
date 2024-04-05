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

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:

    void readClient1();

    void readClient2();

    void processClient(QTcpSocket *client);

    void socketDisconnected1();
    void socketDisconnected2();
    void socketDisconnected();

private:
    QTimer timer;
    int countdownValue;
    QTcpSocket *client1, *client2;
};

#endif // SERVER_H
