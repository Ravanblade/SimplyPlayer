#include "simplygraphicpreloader.h"

SimplyGraphicPreloader::SimplyGraphicPreloader(QObject *parent) :
    QObject(parent)
{

    loadGraphic();

}

void SimplyGraphicPreloader::loadGraphic()
{
    graphicContainer.append(QPixmap(SKIN_PATH + TOP_BORDER));
    graphicContainer.append(QPixmap(SKIN_PATH + DOCK_BORDER));
    graphicContainer.append(QPixmap(SKIN_PATH + LEFT_BORDER));
    graphicContainer.append(QPixmap(SKIN_PATH + RIGHT_BORDER));
    graphicContainer.append(QPixmap(SKIN_PATH + BOTTOM_BORDER));
    graphicContainer.append(QPixmap(SKIN_PATH + CLOSE_N));
    graphicContainer.append(QPixmap(SKIN_PATH + SKIN_ICON));
}


QList<QPixmap> *SimplyGraphicPreloader::getContainer()
{
  return &graphicContainer;
}
