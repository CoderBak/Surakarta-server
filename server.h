#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#include "surakarta/basic.h"
#include "surakarta/game.h"
#include "timer.h"

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

    bool getData(QByteArray &data);

    void updateTimeSlot1(QString time);

    void updateTimeSlot2(QString time);

    void upDateTimeOut();

    //void resetTimerSlot();

private:
    InfoType dataHandler(const QByteArray &info, QTcpSocket *client);

    QTcpSocket *client1, *client2, *currentClient;

    int currentPlayer;

    Player currentPlayerColor;

    std::unique_ptr<Game> game;

    bool retry;
    bool timeOut;

    std::pair<Position, Position> moveMessageHandler(const QByteArray &data);

    Timer *totalTimer;
    Timer *resetTimer;
};

#endif // SERVER_H
