#include "server.h"
#include <iostream>
#include "surakarta/basic.h"
#include "surakarta/game.h"
#include "surakarta/agent/agent_random.h"
#include <QEventLoop>


Server::Server(QObject *parent) : QTcpServer(parent), client1(nullptr), client2(nullptr),
                                  totalTimer(new Timer(this)), resetTimer(new Timer(this)), timeOut(false),
                                  logger(std::make_shared<Logger>()) {
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
        connect(totalTimer, &Timer::updateTime, this, &Server::updateTimeSlot1);
        connect(resetTimer, &Timer::updateTime, this, &Server::updateTimeSlot2);
        connect(resetTimer, &Timer::timeOut, this, &Server::upDateTimeOut);
        connect(totalTimer, &Timer::timeOut, this, &Server::upDateTimeOut);
        totalTimer->start();
        resetTimer->start();
        // connect(resetTimer, &Timer::timeReset, this, &Server::resetTimerSlot);
        startGame();
    }
}

void Server::startGame() {
    isClient1AI = isClient2AI = false;
    retry = false;
    auto clearBuffer = [](auto &client) {
        if (client->bytesAvailable() > 0) {
            client->read(client->bytesAvailable());
        }
    };
    // Start the game loop.
    game = std::make_unique<Game>();
    connect(game.get(), &Game::boardUpdated, this, &Server::onBoardUpdated);
    game->StartGame();
    QString currentTime = QDateTime::currentDateTime().toString("yy-MM-dd-hh-mm-ss");
    currentPlayer = (rand() % 2 + 2) % 2 + 1;
    currentPlayerColor = Player::BLACK;
    if (currentPlayer == 1) {
        client1->write("$SB;"); // Side: B
        client2->write("$SW;"); // Side: W
    } else {
        client1->write("$SW;");
        client2->write("$SB;");
    }
    const auto agent =
            std::make_shared<AgentRandom>(game->GetBoard(), game->GetGameInfo(), game->GetRuleManager());
    while (!game->IsEnd()) {
        qDebug() << isClient1AI << isClient2AI;
        currentClient = (currentPlayer == 1) ? client1 : client2;
        QTcpSocket *otherClient = (currentPlayer == 1) ? client2 : client1;
        if (currentPlayer == 1 && isClient1AI || currentPlayer == 2 && isClient2AI) {
            auto move = agent->CalculateMove();
            logger->addLog(QString("%1;%2->%3;%4 (%5)")
                                   .arg(move.from.x).arg(move.from.y)
                                   .arg(move.to.x).arg(move.to.y)
                                   .arg(currentPlayer));
            game->Move(move);
        } else {
            QEventLoop loop;
            connect(currentClient, &QTcpSocket::readyRead, &loop, &QEventLoop::quit);
            while (true) {
                loop.exec();
                if (currentClient->bytesAvailable() > 0) {
                    QByteArray data = currentClient->read(currentClient->bytesAvailable());
                    if (!getData(data, false)) {
                        break;
                    }
                    if (retry) {
                        totalTimer->reset();
                        resetTimer->reset();
                        break;
                    }
                    // if(timeOut){
                    //     break;
                    // }
                }
            }
        }
        clearBuffer(currentClient);
        if (otherClient->bytesAvailable() > 0) {
            QByteArray data = otherClient->read(otherClient->bytesAvailable());
            getData(data, true);
        }
        clearBuffer(otherClient);
        if (retry) {
            break;
        }
        // if(timeOut){
        //     timeOut=false;
        //     break;
        // }
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        currentPlayerColor = ReverseColor(currentPlayerColor);
        std::cerr << *game->GetGameInfo() << std::endl;
        std::cerr << *game->GetBoard() << std::endl;
    }
    qDebug() << "GAME ENDS!";
    std::cerr << *game->GetBoard();
    std::cerr << game->GetGameInfo()->endReason << std::endl;
    std::cerr << game->GetGameInfo()->winner << std::endl;
    logger->save(currentTime, client1, client2);
    if (retry) {
        startGame();
    }
}

bool Server::getData(QByteArray &data, bool reversed) {
    bool hasMove = false;
    qDebug() << "Now is client" << currentPlayer;
    //qDebug() << "Received message from client: " << data;
    if (data[0] != '$') {
        qDebug() << "Wrong format!";
        return true;
    }
    auto packetHandler = [&](auto packet) {
        const auto info = dataHandler(packet, reversed);
        if (info == InfoType::RETRY) {
            retry = true;
        }
        if (info == InfoType::MOVE) {
            hasMove = true;
        }
    };
    while (true) {
        qDebug() << data;
        const auto endPos = data.indexOf('$', 1);
        if (endPos != -1) {
            packetHandler(data.mid(1, endPos - 1));
            if (hasMove) {
                return false;
            }
            data.remove(0, endPos);
        } else {
            data.remove(0, 1);
            packetHandler(data);
            if (hasMove) {
                return false;
            }
            break;
        }
    }
    return true;
}

void Server::updateTimeSlot1(QString time) {
    // qDebug()<<time<<"server first time";
    QTime startTime = QTime::fromString(time);
    // qDebug()<<startTime<<"starttime";
    startTime = startTime.addSecs(1);
    QString formattedTime = startTime.toString("hh:mm:ss");
    QByteArray timeData = formattedTime.toUtf8();
    QByteArray message;
    message.append("$TIME:T");
    message.append(timeData);
    // qDebug()<<message;
    client1->write(message);
    client2->write(message);
}

void Server::updateTimeSlot2(QString time) {
    // qDebug()<<time<<"server first time";
    QTime startTime = QTime::fromString(time);
    // qDebug()<<startTime<<"starttime";
    startTime = startTime.addSecs(1);
    QString formattedTime = startTime.toString("hh:mm:ss");
    QByteArray timeData = formattedTime.toUtf8();
    QByteArray message;
    message.append("$TIME:R");
    message.append(timeData);
    // qDebug()<<message;
    client1->write(message);
    client2->write(message);
}

void Server::upDateTimeOut() {
    if (resetTimer->getTime() > maxSecond) {
        QByteArray message1, message2;
        message1.append("$ET");
        message2.append("$EF");
        if (currentClient == client1) {
            client1->write(message1);
            client2->write(message2);
            game->gameInfo->winner = ReverseColor(currentPlayerColor);
        } else {
            client2->write(message1);
            client1->write(message2);
            game->gameInfo->winner = ReverseColor(currentPlayerColor);
        }
        totalTimer->reset();
        resetTimer->reset();
        totalTimer->stop();
        resetTimer->stop();
        qDebug() << "Time out!";
        // timeOut=true;
        auto clearBuffer = [](auto &client) {
            if (client->bytesAvailable() > 0) {
                client->read(client->bytesAvailable());
            }
        };
        clearBuffer(client1);
        clearBuffer(client2);
        qDebug() << "GAME ENDS!";
        std::cerr << *game->GetBoard();
        std::cerr << game->GetGameInfo()->endReason << std::endl;
        std::cerr << game->GetGameInfo()->winner << std::endl;
    }
}

void Server::listenPort() {
    server1->listen(QHostAddress::Any, this->port);
}

void Server::receiveFromClient(QTcpSocket *client, NetworkData data) {
    if (data.op == OPCODE::LEAVE_OP) {
        removeClient(client);
        return;
    }
    if (!clients.contains(client)) {
        if (clients.size() >= maxClients) {
            //QMessageBox::warning(this, "Warning", "The server is full!");
            qDebug() << "Server is full!";
            return;
        }
        clients.insert(client);
        if (!client1) {
            client1 = client;
        } else if (!client2) {
            client2 = client;
        }
    }

    if (client == client1) {
        if (client2 && data.op == OPCODE::CHAT_OP)
            sendToAnotherClient(client2, data);
    } else if (client == client2) {
        if (client1 && data.op == OPCODE::CHAT_OP)
            sendToAnotherClient(client1, data);
    } else
        //QMessageBox::warning(this, "Warning", "Unknown client!");
        qDebug() << "Unknown client!";
}

void Server::restartServer() {
    server1->close();
    clients.clear();
    client1 = nullptr;
    client2 = nullptr;
    disconnect(server1, &NetworkServer::receive, this, &Server::receiveFromClient);
    delete server1;
    server1 = new NetworkServer(this);
    connect(server1, &NetworkServer::receive, this, &Server::receiveFromClient);
}

void Server::removeClient(QTcpSocket *client) {
    if (client == client1) {
        client1 = nullptr;
    } else if (client == client2) {
        client2 = nullptr;
    }
    clients.remove(client);
}

void Server::sendToAnotherClient(QTcpSocket *another, NetworkData data) {
    this->server1->send(another, data);
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
    resetTimer->reset();
    return std::make_pair(Position(idx[0], idx[1]), Position(idx[2], idx[3]));
}

InfoType Server::dataHandler(const QByteArray &info, bool reversed) {
    switch (info[0]) {
        case 'A': {
            if (reversed ^ (currentClient == client1)) {
                isClient1AI = (info[1] == '1');
            } else {
                isClient2AI = (info[1] == '1');
            }
            return InfoType::DEFAULT;
        }
        case 'M': {
            if (reversed) {
                return InfoType::DEFAULT;
            }
            auto [from, to] = moveMessageHandler(info);
            auto currentMove = Move(from, to, currentPlayerColor);
            logger->addLog(QString("%1;%2->%3;%4 (%5)").arg(from.x).arg(from.y).arg(to.x).arg(to.y).arg(currentPlayer));
            game->Move(currentMove);
            std::cerr << *game->GetGameInfo() << std::endl;
            std::cerr << *game->GetBoard() << std::endl;
            return InfoType::MOVE;
        }
        case 'Q': {
            if (reversed) {
                return InfoType::DEFAULT;
            }
            // If we want valid capture.
            if (info[1] == 'C') {
                QByteArray dataCopy = info;
                dataCopy.remove(0, 2);
                QList<int> idx;
                QList<QByteArray> dataList = dataCopy.split(';');
                for (const QByteArray &slice: dataList) {
                    idx.append(slice.toInt());
                }
                auto eatable = game->searchEatable(Position(idx[0], idx[1]));
                QString data = QString("$RC%1").arg(eatable.size());
                for (const auto &elem: eatable) {
                    data += QString("|%1;%2").arg(elem.first.y).arg(elem.first.x);
                    for (const auto &piece: elem.second) {
                        data += QString("@%1;%2").arg(piece.y).arg(piece.x);
                    }
                }
                currentClient->write(data.toUtf8());
            } else {
                QByteArray dataCopy = info;
                dataCopy.remove(0, 2);
                QList<int> idx;
                QList<QByteArray> dataList = dataCopy.split(';');
                for (const QByteArray &slice: dataList) {
                    idx.append(slice.toInt());
                }
                auto movable = game->searchMovable(Position(idx[0], idx[1]));
                QString data = QString("$RN%1").arg(movable.size());
                for (const auto &elem: movable) {
                    data += QString("|%1;%2").arg(elem.y).arg(elem.x);
                }
                currentClient->write(data.toUtf8());
            }
            return InfoType::DEFAULT;
        }
        case 'G': {
            if (reversed) {
                return InfoType::DEFAULT;
            }
            return InfoType::RETRY;
        }
        default:
            return InfoType::DEFAULT;
    }
}

void Server::socketDisconnected1() {
    if (client1) {
        // totalTimer->stop();
        // resetTimer->stop();
        qDebug() << "Game stopped because client 1 stopped";
    }
    socketDisconnected();
}

void Server::socketDisconnected2() {
    if (client2) {
        // totalTimer->stop();
        // resetTimer->stop();
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
    totalTimer->stop();
    resetTimer->stop();
    //totalTimer->deleteLater();
    //totalTimer = nullptr;
    //resetTimer->deleteLater();
    //resetTimer = nullptr;

}

void Server::onBoardUpdated(const QString &boardInfo) {
    qDebug() << "Board Updated!";
    qDebug() << boardInfo << "\n";
    client1->write("$B" + boardInfo.toUtf8());
    client2->write("$B" + boardInfo.toUtf8());
    emit boardUpdated(boardInfo);
}
