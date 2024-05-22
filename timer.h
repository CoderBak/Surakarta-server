// In this file, we define the Timer class.
#ifndef TIMER_H
#define TIMER_H

#include <QThread>
#include <QString>
#include <QDateTime>
#include "surakarta/basic.h"

class Timer : public QThread {
Q_OBJECT

public:
    explicit Timer(QObject *parent = nullptr) : QThread(parent), m_running(true), isReset(false), isCheck(false) {}
    void start() { m_running = true; QThread::start(); }
    void stop() { m_running = false; }
    void reset() {
        isReset = true;
        qDebug() << "Reset Timer triggered.";
        startTime = QTime(0, 0, 0);
        qDebug() << startTime << "reset";
    }
    int getTime() { return startTime.second(); }
    void setCheck() { isCheck = true; }

signals:
    void updateTime(QString time);
    void timeOut();

protected:
    void run() override {
        while (m_running) {
            QString timeString = startTime.toString("hh:mm:ss");
            emit updateTime(timeString);
            emit timeOut();
            startTime = startTime.addSecs(1);
            sleep(1);
        }
    }

private:
    QTime startTime = QTime(0, 0, 0);
    bool m_running, isReset, isCheck;
};

#endif // TIMER_H
