#include "coverflow.h"
CoverFlow::CoverFlow(QWidget *parent):
    PictureFlow(parent)
{
    // setWindowTitle("PictureFlow");

}

void CoverFlow::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Escape || event->key() == Qt::Key_Enter ||
            event->key() == Qt::Key_Return)
    {
        event->accept();
        close();
    }

    // checking the speed of rendering
    if(event->key() == Qt::Key_F10)
        if(event->modifiers() == Qt::AltModifier)
        {
            qDebug("benchmarking.... please wait");
            const int blit_count = 10;

            QTime stopwatch;
            stopwatch.start();
            for(int i = 0; i < blit_count; i++)
            {
                render(); repaint(); QApplication::flush(); //QApplication::syncX();
                render(); repaint(); QApplication::flush();// QApplication::syncX();
                render(); repaint(); QApplication::flush();// QApplication::syncX();
                render(); repaint(); QApplication::flush();// QApplication::syncX();
                render(); repaint(); QApplication::flush();// QApplication::syncX();
                render(); repaint(); QApplication::flush(); //QApplication::syncX();
                render(); repaint(); QApplication::flush(); //QApplication::syncX();
                render(); repaint(); QApplication::flush();// QApplication::syncX();
                render(); repaint(); QApplication::flush(); //QApplication::syncX();
                render(); repaint(); QApplication::flush(); //QApplication::syncX();
            }
            QString msg;
            int elapsed = stopwatch.elapsed();
            if( elapsed > 0 )
                msg = QString("FPS: %1").arg( blit_count*10*1000.0/elapsed );
            else
                msg = QString("Too fast. Increase blit_count");
            setWindowTitle( msg );
            event->accept();
            return;
        }

    // for debugging only: Alt+F11 cycles the reflection effect
    if(event->key() == Qt::Key_F11)
        if(event->modifiers() == Qt::AltModifier)
        {
            qDebug("changing reflection effect...");
            switch(reflectionEffect())
            {
            //case NoReflection:      setReflectionEffect(PlainReflection); break;
            case PlainReflection:   setReflectionEffect(BlurredReflection); break;
            case BlurredReflection: setReflectionEffect(PlainReflection); break;
            default:                setReflectionEffect(PlainReflection); break;
            }
            event->accept();
            return;
        }

    PictureFlow::keyPressEvent(event);
}
