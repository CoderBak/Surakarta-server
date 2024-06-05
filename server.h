// This file defines the Server class, which manages network connections and game state.
#ifndef SERVER_H
#define SERVER_H

#include "surakarta/basic.h"
#include "surakarta/game.h"
#include "timer.h"
#include "utils/logger.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>

class Server : public QTcpServer {
Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    void startGame();

signals:
    // Signal emitted when the game board is updated.
    void boardUpdated(const QString &boardInfo);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void socketDisconnected1();
    void socketDisconnected2();
    void socketDisconnected();
    void onBoardUpdated(const QString &boardInfo);
    bool getData(QByteArray &data, bool reversed);
    void updateTimeSlot1(const QString& time);
    void updateTimeSlot2(const QString& time);
    void upDateTimeOut();

private:
    InfoType dataHandler(const QByteArray &info, bool reversed);
    QTcpSocket *client1, *client2, *currentClient;
    int currentPlayer;
    Player currentPlayerColor;
    std::unique_ptr<Game> game;
    bool retry, timeOut;
    std::pair<Position, Position> moveMessageHandler(const QByteArray &data);
    Timer *totalTimer, *resetTimer;
    bool isClient1AI, isClient2AI;
    std::shared_ptr<Logger> logger;
};

#endif // SERVER_H
