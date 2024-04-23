#include "timerthread.h"
#include <QDateTime>

TimerThread::TimerThread(QObject *parent) : QThread(parent), m_running(true) {}

void TimerThread::run() {
    while (m_running) {
        QDateTime currentTime = QDateTime::currentDateTime();
        QString timeString = currentTime.toString("hh:mm:ss");
        emit updateTime(timeString);
        sleep(1);
    }
}

void TimerThread::stop() {
    m_running = false;
}
