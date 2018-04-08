#ifndef QSkinObject_H
#define QSkinObject_H
#include "qskinwidgetresizehandler.h"
#include <QtWidgets>
#ifdef WIN32
#define _WIN32_WINNT 0x0500
#define WINVER 0x0500
#include <windows.h>
#endif
#include "simplyplayer.h"


/*** SKIN FILE NAMES ***/
#define TEMP_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//temp//")
#define SKIN_PATH QDir::fromNativeSeparators(QApplication::applicationDirPath() + "//data//temp//skin//")
#define TOP_BORDER "top.png"
#define DOCK_BORDER "dock.png"
#define LEFT_BORDER "right.png"
#define RIGHT_BORDER "left.png"
#define BOTTOM_BORDER "bottom.png"
#define MIN_N "min_normal.png"
#define MIN_H "min_hover.png"
#define MIN_P "min_pressed.png"
#define MAX_N "max_normal.png"
#define MAX_H "max_hover.png"
#define MAX_P "max_pressed.png"
#define CLOSE_N "close_normal.png"
#define CLOSE_H "close_hover.png"
#define CLOSE_P "close_pressed.png"
#define BAR_N "bar_normal.png"
#define SKIN_ICON "icon.png"
/**********************/

enum SKIN_BUTTON
{
    CLOSE_BUTTON = 0,
    MINIMIZE_BUTTON = 1,
    MAXIMIZE_BUTTON = 2,
    MENU_BUTTON = 3
};


class QSkinWidgetResizeHandler;
class  QSkinObject : public QWidget
{
    Q_OBJECT
    friend class QSkinWidgetResizeHandler;

public:
    QSkinObject(QWidget* wgtParent, SimplyGraphicPreloader *graphic, bool mainWindow);
	~QSkinObject(){}
	void setSkinPath(const QString & skinpath);
	QString getSkinPath();
	int customFrameWidth();
    double frameAlpha;
    QSize sizeHint() const;
    void setLabelName(QString name);
    void hideButton(SKIN_BUTTON button, bool hide);
    QSize getBarSize();
    void setDockBorder(bool dk);
    void setFrameAlpha(float alpha);
    float getFrameAlpha();
    QSkinWidgetResizeHandler *getResizeHandler();
    QList<QPixmap> *graphics;

signals:
    void barButtonClicked();
    void closeClicked();
    void minClicked();
    void maxClicked();

public slots:
	void updateStyle();
    void startSkinning();
	void stopSkinning();
    void onClose();
    void onMin();
    void onMax();
    void onBarButton();

protected:
	bool eventFilter(QObject *o, QEvent *e);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *e);

    bool init;
    QLabel *newLabel;
    QPushButton *closeButton;
    QPushButton *miniButton;
    QPushButton *maxiButton;
    QPushButton *barButton;
	void loadSkinIni();
    QPixmap so;
    QPixmap sl;
    QPixmap sr;
    QPixmap su;
    QPixmap icon;
    QPixmap min;
    QPixmap max;
    QPixmap qui;
    QPixmap barButtonSize;
    QPainter p;
    int temp_w, temp_h;

private:
    QPoint dragPosition;
    QPixmap widgetMask;//the pixmap, in which the ready frame is stored on pressed?
	QString skinPath;
	QFont titleFont;
	QColor titleColor;
    QColor titleGlowColor;
	QColor backgroundColor;
	QRegion quitButton;
	QRegion maxButton;
	QRegion minButton;
	QRect contentsRect;
	QSkinWidgetResizeHandler * resizeHandler;
	QWidget *skinWidget;
	Qt::WindowFlags flags;
    bool isMainWindow;
    bool dockBorder;

};
#endif

