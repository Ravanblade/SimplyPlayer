#ifndef SIMPLYHINT_H
#define SIMPLYHINT_H

#include <QDialog>
#include <QFile>
#include "simplydefinitions.h"

namespace Ui {
class SimplyHint;
}


class SimplyHint : public QDialog
{
    Q_OBJECT
    
public:
    explicit SimplyHint(QWidget *parent = 0);
    ~SimplyHint();
    void Start(QString title, int refresh_speed);
    QTimer *timer2;
    STATE state;
    double alpha;
    QRect size;
    #ifdef WIN32
    bool IsTaskBarOnBottom();
    #endif

public slots:
    void appearEvent();

private:
    Ui::SimplyHint *ui;
};

#endif // SIMPLYHINT_H
