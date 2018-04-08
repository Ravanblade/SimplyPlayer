#include "simplyexpander.h"

SimplyExpander::SimplyExpander(QWidget *parent, QString text) :
    QWidget(parent)
{
    frame = new QFrame(this);
    expander = new QPushButton(frame);
    label = new QLabel(frame);
    widget = new QFrame(this);
  //  widget->setFrameShape(QFrame::Box);
    label->setText(text);
    expander->resize(15,15);
    expander->move(6,1);
    label->move(26,1);
    widget->resize(290,30);
    this->setFixedHeight(30);
    widget->move(6,16);
    expander->setText("+");
   // frame->setFrameShape(QFrame::Box);
    this->resize(290,160);
    isExpand = false;
    e_w = 15;
    e_h = 11;
    tempW = 0;
    widget->setHidden(true);
    connect(expander,SIGNAL(clicked()),this,SLOT(buttonSlot()));
}

void SimplyExpander::setLabelText(QString text)
{
    label->setText(text);
}

void SimplyExpander::setExapndDimensions(int w, int h)
{

   e_w = w;
   e_h = h;

   if(w < 15)
       w = 15;

   if(h < 15)
       h = 15;

   //if(isExpand)
   //    this->resize(w,h);


}


void SimplyExpander::addWidget(QWidget *wid)
{
    wid->setParent(widget);
   // wid->resize(this->width(),widget->height()+15);
    //wid->setFixedHeight(30);
    if(tempW == 0)
    {
        wid->move(5,0);
        tempW = wid->width();
    }
    else
    {
        wid->move(5,tempW);
    }

    //QList<QWidget *> widgets = widget->findChildren<QWidget *>();

   // widgets[0]->move(0,-40);
    //wid->resize(400,400);

}


void SimplyExpander::buttonSlot()
{

    if(isExpand)
        collapse();
    else
        expand();

}


void SimplyExpander::expand()
{
   // this->parentWidget()->resize(this->parentWidget()->width(),this->parentWidget()->height()+e_h);
    expander->setText("-");
    isExpand = true;
   // this->resize(this->width(),this->height()+e_h);
    widget->setHidden(false);
    widget->setFixedHeight(110);
    widget->setFixedWidth(290);
    this->setFixedWidth(290);
    this->setFixedHeight(115);
    this->parentWidget()->repaint();
    emit expandClicked();
}


void SimplyExpander::collapse()
{
   // this->parentWidget()->resize(this->parentWidget()->width(),this->parentWidget()->height()-e_h);
    expander->setText("+");
    isExpand = false;
    widget->setHidden(true);
    this->setFixedHeight(30);
    widget->setFixedHeight(30);
  //  this->resize(this->width(),this->height()-e_h);
   // widget->resize(widget->width(),widget->height()-e_h);
    this->parentWidget()->repaint();
    emit collapseClicked();
}

bool SimplyExpander::isExpanded()
{
    return isExpand;
}




