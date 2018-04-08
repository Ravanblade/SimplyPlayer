#include "simplyhint.h"
#include "ui_simplyhint.h"
#include <QDesktopWidget>
#include <QTimer>
#include <QMessageBox>
#include "simplyfunctions.h"

#ifdef WIN32
#include <windows.h>
#endif

SimplyHint::SimplyHint(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimplyHint)
{
    ui->setupUi(this);

#ifdef WIN32


  //ShowWindow(this->winId(),SW_HIDE);
#endif
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //setAttribute();
    setAttribute(Qt::WA_PaintOnScreen);
   //setWindowFlags();
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    size = QApplication::desktop()->rect();
    int Size = 0, Size2;

    #ifdef WIN32
    if(IsTaskBarOnBottom())
    {
        RECT R;
        SystemParametersInfo(SPI_GETWORKAREA,0,&R,0);
        Size = (size.height() - R.bottom + R.top);
        Size2 = (size.width() - R.left + R.right);
    }
    #endif

   // QMessageBox::about(0,QString::number(size.width()-this->width()),QString::number(size.height()-this->height()+Size));
    this->move(size.height()-this->height()+Size-500,size.width()-this->width()-500);
   //this->move(100,100);

    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(appearEvent()));
    this->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("Hint_Window.qss")));


}

#ifdef WIN32
bool SimplyHint::IsTaskBarOnBottom()
{
  /*HWND hTaskbar;
  RECT *T;
  int scrW, scrH;
  hTaskbar = FindWindow((LPCWSTR)QString("Shell_TrayWnd").utf16(), 0);

  if(hTaskbar != 0)
  {
    GetWindowRect(hTaskbar, (RECT*)T);
    scrW = size.width();
    scrH = size.height();

    if((T->top > scrH/2) && (T->right >= scrW))
      return true; //Na dole

    else if((T->top < scrH/2) && (T->bottom <= scrW/2))
      return false; //Na górze

    else if((T->left < scrW/2) && (T->top <= 0))
      return false; //Z lewej

    else if(T->left >= scrW/2)
      return false; //Na prawej

  }
*/return true;
}
#endif

void SimplyHint::Start(QString title, int refresh_speed)
{
    size = QApplication::desktop()->rect();
    int Size = 0, Size2;
#ifdef WIN32
    if(IsTaskBarOnBottom())
    {
        RECT R;
        SystemParametersInfo(SPI_GETWORKAREA,0,&R,0);
        Size = (size.height() - R.bottom + R.top);
        Size2 = (size.width() - R.left + R.right);
    }
#endif


    this->move((size.width()-this->width()),(size.height()-this->height()-Size));
    //QMessageBox::about(0,QString::number(this->geometry().y()),QString::number(this->geometry().x()));
    //QMessageBox::about(0,"y = " + QString::number(size.width()-this->width()),"x = " + QString::number(size.height()-this->height()+Size));
    //this->move();
    //this->move(1200,700);
    ui->label->setText(title);
    this->timer2->start(refresh_speed);
    state = APPEARING;

}


void SimplyHint::appearEvent()
{


    if(state == APPEARING)
    {
        alpha += 0.01;
        this->setWindowOpacity(alpha);
        if(alpha >= 1)
            state = DISAPPEARING;
    }
    else if(state == DISAPPEARING)
    {
        alpha -= 0.01;
        this->setWindowOpacity(alpha);

        if(alpha <= 0)
            state = EXIT;
    }
    else if(state == EXIT)
    {
        //this->hide();
        this->move(-50,-50);
        this->timer2->stop();
    }



}


SimplyHint::~SimplyHint()
{
    delete ui;
}
