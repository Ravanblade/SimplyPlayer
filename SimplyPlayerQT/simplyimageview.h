#ifndef SIMPLYIMAGEVIEW_H
#define SIMPLYIMAGEVIEW_H

#include <QGraphicsView>

class SimplyImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SimplyImageView(QWidget *parent = 0);
    
signals:
    void doubleClickedImage();

public slots:


protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    
};

#endif // SIMPLYIMAGEVIEW_H
