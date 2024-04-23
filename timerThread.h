#ifndef TIMERTHREAD_H
#define TIMERTHREAD_H

#include <QThread>
#include <QString>

class TimerThread : public QThread {
    Q_OBJECT

public:
    TimerThread(QObject *parent = nullptr);
    void stop();

signals:
    void updateTime(QString time);

protected:
    void run() override;

private:
    bool m_running;
};

#endif // TIMERTHREAD_H
