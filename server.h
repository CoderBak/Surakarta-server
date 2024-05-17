#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QTimer>
#include "surakarta/basic.h"
#include "surakarta/game.h"
#include "timer.h"
#include "NetworkLibrary/networkdata.h"
#include "NetworkLibrary/networkserver.h"
#include "utils/logger.h"
#include "boardUi.h"


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

    bool getData(QByteArray &data, bool reversed);

    void updateTimeSlot1(QString time);

    void updateTimeSlot2(QString time);

    void upDateTimeOut();

    void listenPort();

    void receiveFromClient(QTcpSocket *client, NetworkData data);

    void restartServer();

    void onChessChanged(ChessColor CC[BOARD_SIZE][BOARD_SIZE]);

    //void updateBoard();

    //void resetTimerSlot();

private:
    int port = 1233;
    const int maxClients = 2;
    NetworkServer *server1;

    void removeClient(QTcpSocket *client);

    void sendToAnotherClient(QTcpSocket *another, NetworkData data);

    QSet<QTcpSocket *> clients;

    InfoType dataHandler(const QByteArray &info, bool reversed);

    QTcpSocket *client1, *client2, *currentClient;

    int currentPlayer;

    Player currentPlayerColor;

    std::unique_ptr<Game> game;

    bool retry;
    bool timeOut;

    std::pair<Position, Position> moveMessageHandler(const QByteArray &data);

    Timer *totalTimer;
    Timer *resetTimer;

    bool isClient1AI, isClient2AI;

    std::shared_ptr<Logger> logger;
};

#endif // SERVER_H
