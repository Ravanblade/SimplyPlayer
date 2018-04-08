#include <QtWidgets>
#include <cmath>
#include "qskinobject.h"
#include <QGraphicsDropShadowEffect>

QSkinObject::QSkinObject( QWidget* wgtParent, SimplyGraphicPreloader *graphic, bool mainWindow = true)
    : QWidget()
   
{
    graphics = graphic->getContainer();
	skinWidget = wgtParent;
	contentsRect = skinWidget->rect();
    //qDebug() << contentsRect;
	flags = skinWidget->windowFlags();
    skinWidget->setAutoFillBackground(false);
	widgetMask = QPixmap(skinWidget->width()+12, skinWidget->height()+12);
    init = false;
    dockBorder = false;
   // qDebug() << contentsMargins();
}

void QSkinObject::setDockBorder(bool dk)
{
    if(dk && init)
    {
        if(!dockBorder)
        {
            so = graphics->at(1);
            updateStyle();
            closeButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("CloseButton_d.qss")));
            miniButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("MinimizeButton_d.qss")));
            maxiButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("MaximizeButton_d.qss")));
            dockBorder = true;
            //repaint();
            skinWidget->repaint();
            //this->update();
        }
    }
    else
    {
        if(dockBorder)
        {
            so = graphics->at(0);
            updateStyle();
            closeButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("CloseButton.qss")));
            miniButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("MinimizeButton.qss")));
            maxiButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("MaximizeButton.qss")));
           // repaint();
           // this->update();
            skinWidget->repaint();
            dockBorder = false;
        }
    }

    this->repaint();

}

void QSkinObject::setLabelName(QString name)
{

    newLabel->setText(name);
    newLabel->setHidden(false);
   // if(barButton->isHidden())
        newLabel->move(6,so.height()/2-newLabel->height()+6);
        newLabel->resize(name.size()*5,newLabel->height());
   // else

    //    newLabel->move(sl.width()+barButton->width(),so.height()/2-newLabel->height()+15);
      //  this->newLabel->setText(name);
}

void QSkinObject::hideButton(SKIN_BUTTON button, bool hide)
{

    switch(button)
    {
        case CLOSE_BUTTON:
        {
            this->closeButton->setHidden(hide);
            break;
        }

        case MINIMIZE_BUTTON:
        {
            this->miniButton->setHidden(hide);
            break;
        }

        case MAXIMIZE_BUTTON:
        {

            this->maxiButton->setHidden(hide);
            break;
        }

        case MENU_BUTTON:
        {
            this->barButton->setHidden(hide);
            break;
        }

        default:
        break;

    }


}

void QSkinObject::startSkinning()
{

    newLabel = new QLabel(skinWidget);
    loadSkinIni();

    QPoint pos = skinWidget->pos();
    skinWidget->setAttribute(Qt::WA_PaintOnScreen);
    skinWidget->setAttribute(Qt::WA_NoSystemBackground);

    //no window frame
    skinWidget->setWindowFlags(Qt::FramelessWindowHint);
    skinWidget->setAttribute(Qt::WA_TranslucentBackground,true);

    skinWidget->setAutoFillBackground(false);
	
	skinWidget->setMouseTracking(true);
    skinWidget->setContentsMargins ( graphics->at(2).width(), graphics->at(0).height(), graphics->at(3).width(), graphics->at(4).height());
	
    skinWidget->installEventFilter(this);
    resizeHandler =  new QSkinWidgetResizeHandler(this, skinWidget);

    closeButton = new QPushButton(skinWidget);
    miniButton = new QPushButton(skinWidget);
    maxiButton = new QPushButton(skinWidget);
    barButton = new QPushButton(skinWidget);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(onClose()));
    connect(barButton, SIGNAL(clicked()), this, SLOT(onBarButton()));
    connect(miniButton, SIGNAL(clicked()), this, SLOT(onMin()));
    connect(maxiButton, SIGNAL(clicked()), this, SLOT(onMax()));

    newLabel->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("TitleLabel.qss")));
    closeButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("CloseButton.qss")));
    miniButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("MinimizeButton.qss")));
    maxiButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("MaximizeButton.qss")));
    barButton->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("BarButton.qss")));

    barButton->setGeometry(QRect(0, 0, barButtonSize.width(), barButtonSize.height()));
    closeButton->setGeometry(QRect(skinWidget->width() - qui.width() - sr.width() -8, 2, qui.width(), qui.height()));
    maxiButton->setGeometry(QRect(skinWidget->width() - qui.width() - max.width()-8 - sr.width(), 2, max.width(), max.height()));
    miniButton->setGeometry(QRect(skinWidget->width() - qui.width()-max.width() - min.width()-8 - sr.width(), 2, min.width(), min.height()));
    closeButton->move(skinWidget->width() - qui.width() - sr.width() -8, 2);
    maxiButton->move(skinWidget->width() - qui.width() - max.width()-8 - sr.width(), 2);
    miniButton->move(skinWidget->width() - qui.width()-max.width() - min.width()-8 - sr.width(), 2);

    newLabel->setText(skinWidget->windowTitle());
    newLabel->move(sl.width()+barButton->width(),so.height()/2-newLabel->height()+15);

    skinWidget->show();
    skinWidget->move(pos);




}
float QSkinObject::getFrameAlpha()
{
  return frameAlpha;
}

void QSkinObject::setFrameAlpha(float alpha)
{
    frameAlpha = alpha;
    updateStyle();
    repaint();

}

void QSkinObject::onClose()
{
   // skinWidget->close();
    emit closeClicked();
    skinWidget->close();
}

void QSkinObject::onMin()
{
 //   skinWidget->showMinimized();
    emit minClicked();
}

void QSkinObject::onMax()
{
    emit maxClicked();
  //  skinWidget->close();
}


QSize QSkinObject::getBarSize()
{
    QSize size;
    size.setHeight(so.height());
    size.setWidth(so.width());

    return size;
}

void QSkinObject::stopSkinning()
{
	skinWidget->setContentsMargins ( 5, 5, 5, 5);
	skinWidget->clearMask();
	skinWidget->setAttribute(Qt::WA_PaintOnScreen, false);
	skinWidget->setAttribute(Qt::WA_NoSystemBackground, false);
	skinWidget->setWindowFlags(flags);
	skinWidget->removeEventFilter(this);
    skinWidget->removeEventFilter(resizeHandler);
    skinWidget->setMouseTracking(false);
	skinWidget->show();

}

QSkinWidgetResizeHandler *QSkinObject::getResizeHandler()
{
    return resizeHandler;

}

bool QSkinObject::eventFilter(QObject *o, QEvent *e)
{

    if(e->type() == QEvent::Resize)
	{
		resizeEvent(static_cast<QResizeEvent*>(e));
		return false;
	}
    else if(e->type() == QEvent::Show)
    {
      skinWidget->showNormal();
      skinWidget->showNormal();
      return false;
    }

	else if(e->type() == QEvent::Paint)
	{
        paintEvent(static_cast<QPaintEvent*>(e));
        return false;
	}
	else
	{
         return skinWidget->eventFilter(o, e);
	}
}

void QSkinObject::resizeEvent(QResizeEvent *e)
{

   // manageRegions();
    barButton->setGeometry(QRect(0, 0, barButtonSize.width(), barButtonSize.height()));
    closeButton->setGeometry(QRect(skinWidget->width() - qui.width() - sr.width() -8, 2, qui.width(), qui.height()));
    maxiButton->setGeometry(QRect(skinWidget->width() - qui.width() - max.width()-8 - sr.width(), 2, max.width(), max.height()));
    miniButton->setGeometry(QRect(skinWidget->width() - qui.width()-max.width() - min.width()-8 - sr.width(), 2, min.width(), min.height()));
    closeButton->move(skinWidget->width() - qui.width() - sr.width() -8, 2);
    maxiButton->move(skinWidget->width() - qui.width() - max.width()-8 - sr.width(), 2);
    miniButton->move(skinWidget->width() - qui.width()-max.width() - min.width()-8 - sr.width(), 2);

   // e->ignore();
    //QWidget::resizeEvent(e);
}

void QSkinObject::setSkinPath(const QString & skinpath)
{
	skinPath = skinpath;
	skinWidget->update();
	
}

QString QSkinObject::getSkinPath()
{
	return skinPath;
}

void QSkinObject::onBarButton()
{
    emit barButtonClicked();
}

void QSkinObject::updateStyle()
{
  //  if(init == false || temp_w != skinWidget->width() || temp_h != skinWidget->height())
  //  {
	widgetMask = QPixmap(skinWidget->width(), skinWidget->height());
	widgetMask.fill(Qt::transparent);

    p.begin(&widgetMask);

	int lo = skinWidget->width()-(so.width()-68);

	if(lo < 68)
		lo = 68;

    p.setOpacity(frameAlpha);
	p.drawPixmap(QRect(skinWidget->width()-64, 0,64, so.height()), so, QRect(so.width()-64, 0 , 64, so.height()));
	p.drawPixmap(QRect(0, 0, 64, so.height()), so, QRect(0, 0 , 64, so.height()));
	p.drawPixmap(QRect(64, 0, skinWidget->width()-128, so.height()), so, QRect(64, 0, so.width()-128, so.height()));
    p.setOpacity(1);
	//p.drawPixmap(QRect(sl.width(), height()-su.height(), width(), su.height()), su);
	
	p.drawPixmap(QRect(sl.width(), skinWidget->height() - su.height(), skinWidget->width()-sl.width()-sr.width(), su.height()), 
				su, 
				QRect(sl.width(), 0, su.width()-sl.width()-sr.width(), su.height()));
	p.drawPixmap(QRect(0, skinWidget->height() - su.height(), sl.width(), su.height()),
				su, 
				QRect(0, 0 , sl.width(), su.height()));
				
	
	p.drawPixmap(QRect(skinWidget->width()-sr.width(), skinWidget->height() - su.height(), sr.width(), su.height()), 
				su, 
				QRect(su.width()-sr.width(), 0 , sr.width(), su.height()));			
	
	
	
	
	
	p.drawPixmap(QRect(skinWidget->width()-sr.width(), so.height(), sr.width(), skinWidget->height()-so.height()-su.height()), sr);
	
	
	p.drawPixmap(QRect(0, so.height(), sl.width(), skinWidget->height()-su.height()-so.height()), sl);
	
//	p.setFont(titleFont);
    //p.setPen(QPen(titleColor, 2));
  //  p.setPen(QPen(QColor(224,224,224), 2));
  //  p.setRenderHint(QPainter::Antialiasing);
  //  p.drawPixmap(QRect(5,3,so.height()-5,so.height()-5),icon);

    contentsRect = QRect(sl.width(), so.height(), skinWidget->width() - sr.width() - sl.width(), skinWidget->height() - so.height() - su.height());
    //p.setBrush(backgroundColor);
    p.setOpacity(1);

  //  p.setPen(QPen(titleGlowColor, 1));
   // QLabel *newLabel = new QLabel(this);


    p.setBrush(backgroundColor);
    p.setPen(QPen(titleColor, 1));
    //newLabel->font().set

    /*p.drawText(QRect(sl.width()+so.height()-2, 0, skinWidget->width()-qui.width()*3-8-sl.width()-sr.width(), so.height()), Qt::AlignVCenter, skinWidget->windowTitle());
    p.setBrush(backgroundColor);
    p.setPen(QPen(titleColor, 1));

    p.drawText(QRect(sl.width()+so.height()-2, 2, skinWidget->width()-qui.width()*3-8-sl.width()-sr.width()-6, so.height()-6), Qt::AlignVCenter, skinWidget->windowTitle());*/
    p.drawRect(contentsRect);
	p.end();
	skinWidget->setMask(widgetMask.mask());
	
//	updateButtons();
    init = true;
    temp_h = skinWidget->height();
    temp_w = skinWidget->width();
  //  }

}

QSize QSkinObject::sizeHint() const
{
 //return QSize(widgetMask.width(), widgetMask.height());
   return QSize(skinWidget->width(), skinWidget->height()+400);
}

void QSkinObject::loadSkinIni()
{

      skinPath = SKIN_PATH;
      so = graphics->at(0);
      sl = graphics->at(2);
      sr = graphics->at(3);
      su = graphics->at(4);
      qui = graphics->at(5);
      icon = graphics->at(6);
    QSettings s(QApplication::applicationDirPath() + "//data//temp//config.ini", QSettings::IniFormat);
    s.beginGroup("SkinFile");
    backgroundColor = QColor(s.value("Background_R", 50).toInt(),s.value("Background_G", 50).toInt(),s.value("Background_B", 50).toInt(),s.value("Background_Alpha", 255).toInt());
    titleColor = QColor(s.value("TitleColor_R", 255).toInt(),s.value("TitleColor_G", 255).toInt(),s.value("TitleColor_B", 255).toInt(),s.value("TitleColor_Alpha", 255).toInt());
    frameAlpha = s.value("FrameAlpha",1).toDouble();
	QFont font;
    font.fromString(s.value("Title_font", QFont("Tahoma")).toString());
    font.setPixelSize(12);
    font.setStyleStrategy(QFont::PreferAntialias);

	titleFont = font;
    titleGlowColor =  QColor(s.value("TitleGlowColor__R", 255).toInt(),s.value("TitleGlowColor_G", 255).toInt(),s.value("TitleGlowColor_B", 255).toInt(),s.value("TitleGlowColor_Alpha", 255).toInt());

    QGraphicsDropShadowEffect *glowEffect = new QGraphicsDropShadowEffect();
    glowEffect->setColor(titleGlowColor);
    glowEffect->setBlurRadius(s.value("TitleGlowRadius", 4).toInt());
    glowEffect->setOffset(0,0);
    newLabel->setGraphicsEffect(glowEffect);
  //  setFrameAlpha(s.value("MainWindowFrameAlpha", 0.8).toFloat());
    // playlistSkin->setFrameAlpha(settings2.value("PlaylistFrameAlpha", 0.8).toFloat());
   //  albuminfoSkin->setFrameAlpha(settings2.value("AlbumInfoFrameAlpha", 0.8).toFloat());

     min = QPixmap(skinPath + MIN_P);
     qui = QPixmap(skinPath + CLOSE_P);
     max = QPixmap(skinPath + MAX_P);
     barButtonSize = QPixmap(skinPath + BAR_N);


	s.endGroup();

}

void QSkinObject::paintEvent(QPaintEvent *event)
{
    if(init == false || temp_w != skinWidget->width() || temp_h != skinWidget->height())
    {
         updateStyle();
       //  newLabel->resize(40,9);
         p.begin(&widgetMask);
         p.end();
         init = true;

    }

        p.begin(skinWidget);
       // p.setBackgroundMode(Qt::OpaqueMode);
        p.setRenderHint(QPainter::HighQualityAntialiasing);
      //  p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(rect(), Qt::transparent);
        p.drawPixmap(0,0,widgetMask);
        p.end();


     QWidget::paintEvent(event);
}


int QSkinObject::customFrameWidth()
{
   return graphics->at(2).width();
}
