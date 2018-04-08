#ifndef SIMPLYGRAPHICPRELOADER_H
#define SIMPLYGRAPHICPRELOADER_H

#include <QObject>
#include <QMainWindow>
#include <QList>
#include <QPixmap>
#include <QApplication>
#include <QDir>

#include "simplydefinitions.h"
//#include "simplyplayer.h"

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

class SimplyGraphicPreloader : public QObject
{
    Q_OBJECT
public:
    explicit SimplyGraphicPreloader(QObject *parent = 0);
    void loadGraphic();
    QList<QPixmap> *getContainer();

private:
    QList<QPixmap> graphicContainer;

signals:

public slots:

};

#endif // SIMPLYGRAPHICPRELOADER_H
