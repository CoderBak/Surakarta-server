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
    auto game = std::make_unique<Game>();
    game->StartGame();
    int currentPlayer = (rand() % 2 + 2) % 2 + 1;
    Player current_player_color = Player::BLACK;
    connect(game.get(), &Game::boardUpdated, this, &Server::onBoardUpdated);
    if (currentPlayer == 1) {
        client1->write("SB;"); // Side: B
        client2->write("SW;"); // Side: W
    } else {
        client1->write("SW;");
        client2->write("SB;");
    }
    while (!game->IsEnd()) {
        QEventLoop loop;
        const auto client = (currentPlayer == 1) ? client1 : client2;
        connect(client, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
        loop.exec();
        if (client->bytesAvailable() > 0) {
            QByteArray data = client->readAll();
            qDebug() << "Received data from client " << currentPlayer << " : " << data << "\n";
            auto [from, to] = moveMessageHandler(data);
            auto currentMove = Move(from, to, current_player_color);
            game->Move(currentMove);
            std::cerr << *game->GetGameInfo() << std::endl;
            std::cerr << *game->GetBoard() << std::endl;
        }
        auto clearBuffer = [](auto &client) {
            if (client->bytesAvailable() > 0) {
                client->read(client->bytesAvailable());
            }
        };
        clearBuffer(client1);
        clearBuffer(client2);
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        current_player_color = ReverseColor(current_player_color);
    }
    qDebug() << "GAME ENDS!";
    std::cerr << *game->GetBoard();
    std::cerr << game->GetGameInfo()->endReason << std::endl;
    std::cerr << game->GetGameInfo()->winner << std::endl;
}

std::pair<Position, Position> Server::moveMessageHandler(const QByteArray &data) {
    // When perform move, the info format is : M4;5;3;4
    QByteArray dataCopy = data;
    dataCopy.remove(0, 1);
    QList<int> idx;
    QList<QByteArray> dataList = dataCopy.split(';');
    for (const QByteArray &slice: dataList) {
        idx.append(slice.toInt());
    }
    return std::make_pair(Position(idx[0], idx[1]), Position(idx[2], idx[3]));
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
        client1->deleteLater();
        client1 = nullptr;
    }
    if (client2) {
        qDebug() << "Client 2 stopped.";
        client2->deleteLater();
        client2 = nullptr;
    }
}

void Server::onBoardUpdated(const QString &boardInfo) {
    qDebug() << "Board Updated!";
    qDebug() << boardInfo << "\n";
    client1->write(boardInfo.toUtf8());
    client2->write(boardInfo.toUtf8());
}