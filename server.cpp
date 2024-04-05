#include "server.h"
#include <iostream>
#include "surakarta/basic.h"
#include "surakarta/game.h"
#include <QEventLoop>

Server::Server(QObject *parent) : QTcpServer(parent), client1(nullptr), client2(nullptr) {
    if (!listen(QHostAddress::Any, 1234)) {
        qDebug() << "Unable to listen at port 1234.";
        exit(-1);
    } else {
        qDebug() << "Server established!";
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    if (!client1) {
        client1 = new QTcpSocket(this);
        client1->setSocketDescriptor(socketDescriptor);
        connect(client1, &QTcpSocket::disconnected, this, &Server::socketDisconnected1);
        qDebug() << "Client 1 connected.";
    } else if (!client2) {
        client2 = new QTcpSocket(this);
        client2->setSocketDescriptor(socketDescriptor);
        connect(client2, &QTcpSocket::disconnected, this, &Server::socketDisconnected2);
        qDebug() << "Client 2 connected.";
        qDebug() << "2 players ready, start the game now!\n";
        startGame();
    }
}

void Server::startGame() {
    Game game;
    game.StartGame();
    int current_player = 1;
    Player current_player_color = Player::BLACK;
    while (!game.IsEnd()) {
        QEventLoop loop;
        if (current_player == 1) {
            connect(client1, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
        } else {
            connect(client2, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
        }
        loop.exec();
        const auto client = (current_player == 1) ? client1 : client2;
        if (client->bytesAvailable() > 0) {
            QByteArray data = client->readAll();
            qDebug() << "Received data from client " << current_player << " : " << data << "\n";
            QList<int> numbers;
            QList<QByteArray> dataList = data.split(';');
            for (const QByteArray &slice : dataList) {
                numbers.append(slice.toInt());
            }
            auto currentMove = Move(Position(numbers[0], numbers[1]), Position(numbers[2], numbers[3]), current_player_color);
            game.Move(currentMove);
            std::cout << *game.GetGameInfo() << std::endl;
            std::cout << *game.GetBoard() << std::endl;
        }
        auto clearBuffer = [](auto& client) {
            if (client->bytesAvailable() > 0) {
                client->read(client->bytesAvailable());
            }
        };
        clearBuffer(client1);
        clearBuffer(client2);
        current_player = (current_player == 1) ? 2 : 1;
        current_player_color = ReverseColor(current_player_color);
    }
    qDebug() << "GAME ENDS!";
    std::cout << *game.GetBoard();
    std::cout << game.GetGameInfo()->endReason << std::endl;
    std::cout << game.GetGameInfo()->winner << std::endl;
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
