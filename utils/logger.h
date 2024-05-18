#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QTcpSocket>

class Logger {
public:
    void addLog(const QString& msg) {
        qDebug() << "Received log" << msg;
        log += msg + "\n";
    }

    void save(const QString& title, QTcpSocket* client1, QTcpSocket* client2) {
        qDebug() << "Saving log";
        QFile file(title + ".log");
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << log;
            qDebug() << "Log saved";
        }
        client1->write(("$L" + title + log).toUtf8());
        client2->write(("$L" + title + log).toUtf8());
        log.clear();
    }

    QString log;
};

#endif //LOGGER_H
