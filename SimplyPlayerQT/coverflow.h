#ifndef COVERFLOW_H
#define COVERFLOW_H

#include <QWidget>
#include "pictureflow.h"
#include "simplyplayer.h"
#include <QKeyEvent>

#if QT_VERSION < 0x040000
#define modifiers state
#define AltModifier AltButton
#define setWindowTitle setCaption
#endif

#if QT_VERSION < 0x030000
#define flush flushX
#endif

class CoverFlow : public PictureFlow
{
    Q_OBJECT
public:
    CoverFlow(QWidget *parent = 0);
protected:
    void keyPressEvent(QKeyEvent* event);


};


#endif // COVERFLOW_H
