// In this file, we handle logger for server.
#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QTcpSocket>
#include <QDir>
#include <QFile>

class Logger {
public:
    void addLog(const QString& msg) {
        log += msg + "\n";
    }

    void save(const QString& title, QTcpSocket* client1, QTcpSocket* client2) {
        QDir dir;
        if (!dir.exists("./log")) {
            dir.mkpath("./log");
        }
        QFile file("./log/" + title + ".log");
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << log;
            qDebug() << "Log saved";
        } else {
            qDebug() << "Failed to save logs";
        }
        client1->write(("$L" + title + log).toUtf8());
        client2->write(("$L" + title + log).toUtf8());
        log.clear();
    }

    QString log;
};

#endif //LOGGER_H
