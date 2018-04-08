#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include <QWidget>
#include "simplymediaplayer.h"
#include <QMainWindow>
#include <QApplication>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QTimer>
#include <QMouseEvent>
#include <QUrl>
#include <QDebug>
#include <QVariant>
#include <QList>
#include <QPainter>
#include <QSettings>
#include <math.h>

//#include "glwidget.h"
struct SpectrumSettings
{
    float gk, gs;
    float odstepy;
    float h_gk;
    bool grid;
    bool kostka;
    bool reflect;
    bool mirror;
    bool upToDown;
    bool waves;
    QColor gradient_1;
    QColor gradient_2;
    QColor kolor_tla;
    float dropSpeed;
    float updateSpeed;
    float growSpeed;

};


class SpectrumAnalyzer : public QWidget
{
    Q_OBJECT
public:
    explicit SpectrumAnalyzer(QWidget *parent = 0);
    void LoadINI();
    void LoadCustomSettings(SpectrumSettings set);
    QTimer *timer;
    QTimer *timer2;
    void setAnimationMode(int anim);
    void setPlayer(SimplyMediaPlayer *pl);

   // void setTimersEnabled(bool enb);

protected:
    void paintEvent(QPaintEvent *event);
    float gk, gs;
    float odstepy;
    float h_gk;
    bool grid;
    bool kostka;
    bool reflect;
    bool mirror;
    bool upToDown;
    bool waves;
    QColor gradient_1;
    QColor gradient_2;
    QColor kolor_tla;
    int licznik;
    short int numOfGradients;
    int bar_count;
    QPixmap widgetMask;
    QPainter p;
    float dropSpeed;
    float updateSpeed;
    float growSpeed;
    int animation;
    SimplyMediaPlayer *Player;

private:
    float last_fft[1024];
    float fft[1024];
    float h_fft[1024];

    
signals:
    
public slots:
    void TimerEvent();
    void Timer2Event();
    
};

#endif // SPECTRUMANALYZER_H
