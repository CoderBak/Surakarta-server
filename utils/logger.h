#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

class Logger {
public:
    void addLog(const QString& msg) {
        qDebug() << "Received log" << msg;
        log += msg + "\n";
    }

    void save(const QString& title) {
        qDebug() << "Saving log";
        QFile file(title + ".log");
        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);
            stream << log;
            qDebug() << "Log saved";
        }
        log.clear();
    }

    QString log;
};

#endif //LOGGER_H
