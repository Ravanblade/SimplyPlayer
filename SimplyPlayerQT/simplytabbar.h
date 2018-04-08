#ifndef SIMPLYTABBAR_H
#define SIMPLYTABBAR_H

#include <QTabBar>
#include <QTabWidget>
#include <QSettings>
#include <QLabel>
#include <QtWidgets>

class SimplyTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit SimplyTabBar(QWidget *parent = 0);
    void setParentWidget(QTabWidget *wd);
private:
    QTabWidget *parentWidget;

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    
signals:
    
public slots:
    
};

#endif // SIMPLYTABBAR_H
