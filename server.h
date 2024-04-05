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

    void readData();

    void countdown();

    void socketDisconnected();

private:
    QTimer timer;
    int countdownValue;
    QTcpSocket *currentSocket;
};

#endif // SERVER_H
