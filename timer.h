#ifndef TIMER_H
#define TIMER_H

#include <QThread>
#include <QString>
#include <QDateTime>

class Timer : public QThread {
Q_OBJECT

public:
explicit Timer(QObject *parent = nullptr) : QThread(parent), m_running(true) {}

    void start() {
        m_running = true;
        QThread::start();
    }

    void stop() {
        m_running = false;
    }

    void reset(){
        qDebug() << "Reset Timer triggered.";
        startTime = QTime(0, 0, 0);
        qDebug()<<startTime<<"reset";
        // emit timeReset();
    }

signals:

    void updateTime(QString time);
    // void timeReset();

protected:
    void run() override {

        while (m_running) {
            QString timeString = startTime.toString("hh:mm:ss");
            qDebug()<<startTime<<" timer";

            emit updateTime(timeString);
            startTime=startTime.addSecs(1);
            sleep(1);
        }
    }

private:
    QTime startTime=QTime(0,0,0);
    bool m_running;
};

#endif // TIMER_H

