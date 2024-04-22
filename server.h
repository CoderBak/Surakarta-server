#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#include "surakarta/basic.h"
#include "surakarta/game.h"

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

    void onBoardUpdated(const QString &boardInfo);

private:
    InfoType dataHandler(const QByteArray &info);

    QTcpSocket *client1, *client2;

    int currentPlayer;

    Player currentPlayerColor;

    std::unique_ptr<Game> game;

    static std::pair<Position, Position> moveMessageHandler(const QByteArray &data);
};

#endif // SERVER_H
