#include "server.h"
#include <chrono>
#include <iostream>
#include <thread>
#include "surakarta/agent/agent_random.h"
#include "surakarta/basic.h"
#include "surakarta/game.h"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent), countdownValue(0), client1(nullptr), client2(nullptr) {
    if (!listen(QHostAddress::Any, 1234)) {
        qDebug() << "Unable to listen at port 1234.";
        exit(1);
    } else {
        qDebug() << "Server established!";
    }
    Game game;
    game.StartGame();
    const auto agent =
            std::make_shared<AgentRandom>(game.board_, game.game_info_, game.GetRuleManager());
    const auto my_agent =
            std::make_shared<AgentRandom>(game.GetBoard(), game.GetGameInfo(), game.GetRuleManager());
    // if set to 0, start from TA's agent. (we are W)
    // if set to 1, start from our agent. (we are B)
    int current_player = 0;
    while (!game.IsEnd()) {
        auto move = (current_player == 0) ? agent->CalculateMove() : my_agent->CalculateMove();
        game.Move(move);
        std::cout << *game.GetGameInfo() << std::endl;
        std::cout << *game.GetBoard() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(0));
        current_player = 1 - current_player;
    }
    std::cout << *game.GetBoard();
    std::cout << game.GetGameInfo()->endReason << std::endl;
    std::cout << game.GetGameInfo()->winner << std::endl;
}

void Server::incomingConnection(qintptr socketDescriptor) {
    if (!client1) {
        client1 = new QTcpSocket(this);
        client1->setSocketDescriptor(socketDescriptor);
        connect(client1, &QTcpSocket::disconnected, this, &Server::socketDisconnected1);
        connect(client1, &QTcpSocket::readyRead, this, &Server::readClient1);
        qDebug() << "Client 1 connected.";
    } else {
        client2 = new QTcpSocket(this);
        client2->setSocketDescriptor(socketDescriptor);
        connect(client2, &QTcpSocket::disconnected, this, &Server::socketDisconnected2);
        connect(client2, &QTcpSocket::readyRead, this, &Server::readClient2);
        qDebug() << "Client 2 connected.";
    }
}

void Server::readClient1() {
    processClient(client1);
}

void Server::readClient2() {
    processClient(client2);
}

void Server::processClient(QTcpSocket *client) {
    QByteArray data = client->readAll();
    qDebug() << "Received from client" << (client == client1 ? "1:" : "2:") << data;
}

void Server::socketDisconnected1() {
    if (client1) {
        qDebug() << "Game stopped because client 1 stopped";
    }
    socketDisconnected();
}

void Server::socketDisconnected2() {
    if (client2) {
        qDebug() << "Game stopped because client 2 stopped";
    }
    socketDisconnected();
}

void Server::socketDisconnected() {
    if (client1) {
        qDebug() << "Client 1 stopped.";
        client1 -> deleteLater();
        client1 = nullptr;
    }
    if (client2) {
        qDebug() << "Client 2 stopped.";
        client2 -> deleteLater();
        client2 = nullptr;
    }
}
