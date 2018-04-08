#ifndef SIMPLYDOCKWINDOW_H
#define SIMPLYDOCKWINDOW_H

#include <QWidget>

class SimplyDockWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SimplyDockWindow(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *e);
    bool isDocked();
    bool isDockedDown();
    void setDockWidgets(QList<QWidget *> list);
    void setDocked(bool dk);
    void setDockedDown(bool dk);
    void moveF(QPoint point, bool chkDock);
    QWidget *dockedTo();
    void setOldPos(QPoint point);
    void setOldPos(int x, int y);
    QPoint oldPos();


signals:
    void resizing();
    void dockDown();
    
public slots:


protected:
    void moveEvent(QMoveEvent * event);
    void mousePressEvent(QMouseEvent * event);
    bool docked;
    bool dockedDown;
    bool checkDock;
    int oldX;
    int oldY;

private:
    QList<QWidget *> docks;
    QWidget *dockedWidget;
    
};

#endif // SIMPLYDOCKWINDOW_H
