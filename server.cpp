//In this file, we finish the basic function of server
#include "server.h"
#include "surakarta/basic.h"
#include "surakarta/game.h"
#include "surakarta/agent/agent_ai.h"
#include <QEventLoop>
#include <random>

// Server constructor: Initializes the server and sets up listening on port.
Server::Server(QObject *parent) : QTcpServer(parent), client1(nullptr), client2(nullptr),
                                  totalTimer(new Timer(this)), resetTimer(new Timer(this)), timeOut(false),
                                  logger(std::make_shared<Logger>()) {
    if (!listen(QHostAddress::Any, PORT)) {
        qDebug() << "Unable to listen at port.";
        exit(-1);
    } else {
        qDebug() << "Server established!";
    }
}

// Called when a new connection is incoming.
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

        // Connect timers and start them
        connect(totalTimer, &Timer::updateTime, this, &Server::updateTimeSlot1);
        connect(resetTimer, &Timer::updateTime, this, &Server::updateTimeSlot2);
        connect(resetTimer, &Timer::timeOut, this, &Server::upDateTimeOut);
        connect(totalTimer, &Timer::timeOut, this, &Server::upDateTimeOut);
        totalTimer->start();
        resetTimer->start();
        startGame();
    }
}

// Starts the game setup and main game loop.
void Server::startGame() {
    logger->addLog(QString::number(BOARD_SIZE));
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
    // Generate random number.
    std::random_device dev;
    std::mt19937 engine(dev());
    std::uniform_int_distribution<int> distribution(1, 2);
    currentPlayer = distribution(engine);
    currentPlayerColor = Player::BLACK;

    // Notify clients of their sides.
    if (currentPlayer == 1) {
        client1->write("$SB;"); // Side: B
        client2->write("$SW;"); // Side: W
    } else {
        client1->write("$SW;");
        client2->write("$SB;");
    }
    const auto agent =
            std::make_shared<AgentAI>(game->board, game->gameInfo, game->ruleManager);
    while (!game->isEnd()) {
        qDebug() << isClient1AI << isClient2AI;
        currentClient = (currentPlayer == 1) ? client1 : client2;
        QTcpSocket *otherClient = (currentPlayer == 1) ? client2 : client1;
        if (currentPlayer == 1 && isClient1AI || currentPlayer == 2 && isClient2AI) {
            //AI move
            auto move = agent->CalculateMove();
            logger->addLog(QString("%1;%2->%3;%4 (%5)")
                                   .arg(move.from.x).arg(move.from.y)
                                   .arg(move.to.x).arg(move.to.y)
                                   .arg(currentPlayer));
            game->Move(move);
        } else {
            // Human player move
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
        // Handle data from the other client if available.
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
    }
    qDebug() << "GAME ENDS!";
    logger->save(currentTime, client1, client2);
    if (retry) {
        startGame();
    }
}

// Processes incoming data from clients.
bool Server::getData(QByteArray &data, bool reversed) {
    bool hasMove = false;
    qDebug() << "Now is client" << currentPlayer;
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

// Updates time for client
void Server::updateTimeSlot1(const QString &time) {
    QTime startTime = QTime::fromString(time);
    startTime = startTime.addSecs(1);
    QString formattedTime = startTime.toString("hh:mm:ss");
    QByteArray timeData = formattedTime.toUtf8();
    QByteArray message;
    message.append("$TIME:T");
    message.append(timeData);
    client1->write(message);
    client2->write(message);
}

void Server::updateTimeSlot2(const QString &time) {
    QTime startTime = QTime::fromString(time);
    startTime = startTime.addSecs(1);
    QString formattedTime = startTime.toString("hh:mm:ss");
    QByteArray timeData = formattedTime.toUtf8();
    QByteArray message;
    message.append("$TIME:R");
    message.append(timeData);
    client1->write(message);
    client2->write(message);
}

// Handles time-out event
void Server::upDateTimeOut() {
    if (resetTimer->getTime() > maxSecond) {
        QByteArray message1, message2;
        message1.append("$ET");
        message2.append("$EF");

        // Notify clients of the end of the game due to time-out.
        if (currentClient == client1) {
            client1->write(message1);
            client2->write(message2);
            game->gameInfo->winner = ReverseColor(currentPlayerColor);
        } else {
            client2->write(message1);
            client1->write(message2);
            game->gameInfo->winner = ReverseColor(currentPlayerColor);
        }

        // Reset and stop the timers.
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
    }
}

// Handles move message data.
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

// Processes data packets from clients.
InfoType Server::dataHandler(const QByteArray &info, bool reversed) {
    switch (info[0]) {
        case 'A': {
            // AI setting
            if (reversed ^ (currentClient == client1)) {
                isClient1AI = (info[1] == '1');
            } else {
                isClient2AI = (info[1] == '1');
            }
            return InfoType::DEFAULT;
        }
        case 'M': {
            // Move command
            if (reversed) {
                return InfoType::DEFAULT;
            }
            auto [from, to] = moveMessageHandler(info);
            auto currentMove = Move(from, to, currentPlayerColor);
            logger->addLog(QString("%1;%2->%3;%4 (%5)").arg(from.x).arg(from.y).arg(to.x).arg(to.y).arg(currentPlayer));
            game->Move(currentMove);
            return InfoType::MOVE;
        }
        case 'Q': {
            // Query command
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
                // Valid move query
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
            // Retry command
            if (reversed) {
                return InfoType::DEFAULT;
            }
            return InfoType::RETRY;
        }
        default:
            return InfoType::DEFAULT;
    }
}

// Slot function to handle disconnection of client 1.
void Server::socketDisconnected1() {
    if (client1) {
        qDebug() << "Game stopped because client 1 stopped";
    }
    socketDisconnected();
}

// Slot function to handle disconnection of client 2.
void Server::socketDisconnected2() {
    if (client2) {
        qDebug() << "Game stopped because client 2 stopped";
    }
    socketDisconnected();
}

// Handles cleanup and state reset when any client disconnects
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
    totalTimer->reset();
    resetTimer->reset();
}

// Slot function to handle board updates. Send boardInfo to client
void Server::onBoardUpdated(const QString &boardInfo) {
    qDebug() << "Board Updated!";
    qDebug() << boardInfo << "\n";
    client1->write("$B" + boardInfo.toUtf8());
    client2->write("$B" + boardInfo.toUtf8());
    logger->addLog("B" + boardInfo);
    emit boardUpdated(boardInfo);
}
