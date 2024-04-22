#include "server.h"
#include <iostream>
#include "surakarta/basic.h"
#include "surakarta/game.h"
#include <QEventLoop>

Server::Server(QObject *parent) : QTcpServer(parent), client1(nullptr), client2(nullptr) {
    if (!listen(QHostAddress::Any, 1233)) {
        qDebug() << "Unable to listen at port 1233.";
        exit(-1);
    } else {
        qDebug() << "Server established!";
    }
}

void Server::incomingConnection(qintptr socketDescriptor) {
    // Try to connect to 2 clients.
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
    bool retry = false;
    auto clearBuffer = [](auto &client) {
        if (client->bytesAvailable() > 0) {
            client->read(client->bytesAvailable());
        }
    };
    // Start the game loop.
    game = std::make_unique<Game>();
    connect(game.get(), &Game::boardUpdated, this, &Server::onBoardUpdated);
    game->StartGame();
    currentPlayer = (rand() % 2 + 2) % 2 + 1;
    currentPlayerColor = Player::BLACK;
    if (currentPlayer == 1) {
        client1->write("$SB;"); // Side: B
        client2->write("$SW;"); // Side: W
    } else {
        client1->write("$SW;");
        client2->write("$SB;");
    }
    while (!game->IsEnd()) {
        QEventLoop loop;
        const auto client = (currentPlayer == 1) ? client1 : client2;
        connect(client, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
        loop.exec();
        if (client->bytesAvailable() > 0) {
            qDebug() << "Now is client" << currentPlayer;
            QByteArray data = client->readAll();
            qDebug() << "Received message from client: " << data;
            if (data[0] != '$') {
                qDebug() << "Wrong format!";
            } else {
                auto packetHandler = [&](auto packet) {
                    const auto info = dataHandler(packet);
                    if (info == InfoType::RETRY) {
                        retry = true;
                    }
                };
                while (true) {
                    const auto endPos = data.indexOf('$', 1);
                    if (endPos != -1) {
                        packetHandler(data.mid(1, endPos - 1));
                        data.remove(0, endPos);
                    } else {
                        data.remove(0, 1);
                        packetHandler(data);
                        break;
                    }
                }
            }
        }
        if (retry) {
            break;
        }
        clearBuffer(client1);
        clearBuffer(client2);
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        currentPlayerColor = ReverseColor(currentPlayerColor);
    }
    qDebug() << "GAME ENDS!";
    std::cerr << *game->GetBoard();
    std::cerr << game->GetGameInfo()->endReason << std::endl;
    std::cerr << game->GetGameInfo()->winner << std::endl;
    if (retry) {
        startGame();
    }
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

InfoType Server::dataHandler(const QByteArray &info) {
    if (info[0] == 'M') {
        auto [from, to] = moveMessageHandler(info);
        auto currentMove = Move(from, to, currentPlayerColor);
        game->Move(currentMove);
        std::cerr << *game->GetGameInfo() << std::endl;
        std::cerr << *game->GetBoard() << std::endl;
        return InfoType::MOVE;
    } else {
        return InfoType::RETRY;
    }
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
    client1->write("$" + boardInfo.toUtf8());
    client2->write("$" + boardInfo.toUtf8());
}