#ifndef TABWIDGET_H
#define TABWIDGET_H
#include <QTabWidget>
#include <QtWidgets>

class TabWidget : public QTabWidget {
public:
    TabWidget(QWidget *p = 0) : QTabWidget(p){}

public:

    QTabBar *tabBar() const { return QTabWidget::tabBar(); }
    void setTabBar(QTabBar * a) {return QTabWidget::setTabBar(a); }
protected:
    void paintEvent(QPaintEvent *e);
public slots:

};

#endif // TABWIDGET_H
