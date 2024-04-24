#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>
#include <QString>
#include <QDateTime>

class TimerThread : public QThread {
Q_OBJECT

public:
    explicit TimerThread(QObject *parent = nullptr) : QThread(parent), m_running(true) {}

    void stop() {
        m_running = false;
    }


signals:

    void updateTime(QString time);

protected:
    void run() override {
        while (m_running) {
            QDateTime currentTime = QDateTime::currentDateTime();
            QString timeString = currentTime.toString("hh:mm:ss");
            emit updateTime(timeString);
            sleep(1);
        }
    }

private:
    bool m_running;
};

#endif // TIMERTHREAD_H
