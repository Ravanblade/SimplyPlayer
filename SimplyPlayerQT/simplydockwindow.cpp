#include "simplydockwindow.h"
#include "QDebug"
#include "windows.h"

SimplyDockWindow::SimplyDockWindow(QWidget *parent) :
    QWidget(parent)
{

    //this->setWindowFlags(windowFlags() | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground,true);
    resize(1000,1000);

}

void SimplyDockWindow::resizeEvent(QResizeEvent *e)
{

    emit resizing();
    QWidget::resizeEvent(e);
}

bool SimplyDockWindow::isDocked()
{
    return docked;
}

bool SimplyDockWindow::isDockedDown()
{
    return dockedDown;
}

QWidget *SimplyDockWindow::dockedTo()
{
    return dockedWidget;
}

void SimplyDockWindow::setDocked(bool dk)
{
    docked = dk;
}

void SimplyDockWindow::setDockedDown(bool dk)
{
    dockedDown = dk;
}


void SimplyDockWindow::setDockWidgets(QList<QWidget *> list)
{
    docks = list;
}

void SimplyDockWindow::setOldPos(QPoint point)
{
    this->oldX = point.x();
    this->oldY = point.y();
}

void SimplyDockWindow::setOldPos(int x, int y)
{
    this->oldX = x;
    this->oldY = y;
}

QPoint SimplyDockWindow::oldPos()
{
    return QPoint(this->oldX,this->oldY);
}


void SimplyDockWindow::mousePressEvent(QMouseEvent * event)
{
   // QWidget::mouseReleaseEvent(event);
}

void SimplyDockWindow::moveEvent(QMoveEvent * event)
{
   // qDebug() << "Dock x: " << dock->x() << " y:" << dock->y();
   // qDebug() << "Window x: " << this->x() << " y:" << this->y();
    //qDebug() << docked;
    if(checkDock)
    {
   // qDebug() << "MoveEvent: " << docked << "x: " << this->x() << "y: " << this->y();
    for(int i = 0; i < docks.size(); i++)
    {

        if(docks[i] != 0)
        {
          //  qDebug() << "Dock x: " << docks[i]->x() << " y:" << docks[i]->y();
          //  qDebug() << "Window x: " << this->x() << " y:" << this->y();

            bool check = true;
            //PRAWO
            if((this->x()-docks[i]->width() <= docks[i]->x()+10) && (this->x()-docks[i]->width() >= docks[i]->x())
                    && (this->y() <= docks[i]->y()+docks[i]->height() && this->y()+this->height() >= docks[i]->y()))
            {

                docked = true;
                this->move(docks[i]->x()+docks[i]->width(),this->y());
                check = false;
                emit dockDown();
                dockedWidget = docks[i];

                checkDock = true;


                 return;
            }
            //DOL
            else if((this->y()-docks[i]->height() <= docks[i]->y()+10) && (this->y()-docks[i]->height() >= docks[i]->y())
                    && (this->width()+this->x() >= docks[i]->x()) && this->x() <= docks[i]->x()+docks[i]->width())
            {


                docked = true;
                dockedDown = true;
                this->move(this->x(),docks[i]->y()+docks[i]->height());
                check = false;
                emit dockDown();
                dockedWidget = docks[i];
                checkDock = true;

                return;
            }
            //LEWO
            else if((this->x()+this->width() >= docks[i]->x()-10) && (this->x()+this->width() <= docks[i]->x())
                    && (this->y() <= docks[i]->y()+docks[i]->height() && this->y()+this->height() >= docks[i]->y()))
            {

                docked = true;
                check = false;
                this->move(docks[i]->x()-this->width(),this->y());
                emit dockDown();
                dockedWidget = docks[i];

                checkDock = true;

                 return;
            }


        }

        docked = false;
        dockedDown = false;
        dockedWidget = 0;
        emit dockDown();

    }
       // checkDock = false;
    }
    checkDock = true;

}

void SimplyDockWindow::moveF(QPoint point, bool chkDock)
{

    checkDock = chkDock;
    this->move(QPoint(point.x(),point.y()));

}

