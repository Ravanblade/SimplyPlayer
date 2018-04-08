#include "qtimerthread.h"

QTimerThread::QTimerThread(QObject *parent) :
    QThread(parent)
{
}

void QTimerThread::run()
{
    timer = new QTimer();


}
