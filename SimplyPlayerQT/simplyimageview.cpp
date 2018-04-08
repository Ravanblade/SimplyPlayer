#include "simplyimageview.h"

SimplyImageView::SimplyImageView(QWidget *parent) :
    QGraphicsView(parent)
{
}

void SimplyImageView::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleClickedImage();

    QWidget::mouseDoubleClickEvent(event);

}
