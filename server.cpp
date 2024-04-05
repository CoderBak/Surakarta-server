#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QTcpServer(parent), countdownValue(0), currentSocket(nullptr) {
    if (!listen(QHostAddress::Any, 1234)) {
        qDebug() << "Unable to listen at port 1234.";
        exit(1);
    } else {
        qDebug() << "Server established!";
    }
    connect(&timer, &QTimer::timeout, this, &Server::countdown);
}

void Server::incomingConnection(qintptr socketDescriptor) {
    auto *socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::disconnected, this, &Server::socketDisconnected); // 连接disconnected信号
    connect(socket, &QTcpSocket::readyRead, this, &Server::readData);
    currentSocket = socket;
    countdownValue = 0;
    timer.start(1000);
}

void Server::readData() {
    if (currentSocket) {
        QByteArray data = currentSocket->readAll();
        countdownValue = QString(data).toInt();
        qDebug() << "Received number from client:" << countdownValue;
    }
}

void Server::countdown() {
    if (countdownValue > 0) {
        countdownValue--;
        QByteArray data = QString::number(countdownValue).toUtf8();
        if (currentSocket) {
            currentSocket->write(data);
            currentSocket->flush();
        }
    }
}

void Server::socketDisconnected() {
    if (currentSocket) {
        qDebug() << "Client disconnected.";
        timer.stop();
        currentSocket->deleteLater();
        currentSocket = nullptr;
    }
}