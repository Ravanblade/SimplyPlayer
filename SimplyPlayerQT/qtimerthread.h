#ifndef QTIMERTHREAD_H
#define QTIMERTHREAD_H

#include <QThread>
#include <QTimer>

class QTimerThread : public QThread
{
    Q_OBJECT
public:
    explicit QTimerThread(QObject *parent = 0);
    QTimer *timer;
    
signals:
    
public slots:
    
};

#endif // QTIMERTHREAD_H
