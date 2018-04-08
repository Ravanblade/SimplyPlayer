#ifndef PLAYER_H
#define PLAYER_H

#include "bass.h"
//#include "simplyplayer.h"
#include "simplyfunctions.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>
#include <QUrl>
#include <QVariant>
#include <QList>
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include <QThread>



struct PTime
{
  float minutes;
  int seconds;
};

class SimplyMediaPlayer : public QThread
{
    Q_OBJECT

protected:
    void run();

private:
    DWORD MainChannel;
    bool isSpectrumStarted;
    QString LastPath;
    QString LastFileName;
    int LastVolume;
    int LastBalance;
    DWORD TempChannel;
    char *filename;
    bool stopThread;

public:
    SimplyMediaPlayer(QWidget *parent = 0);
    virtual ~SimplyMediaPlayer();
    int Init();
    int InitEqualizer();
    QPixmap DrawSpectrum(QWidget *ui, int x, int y, int w, int h, bool returnOnly = false);
   // void RedrawSpectrum(QWidget *ui);
    QPixmap GetLastSpectrumRender(QWidget *ui);
    DWORD GetChannel();
    int LoadFile(QString files);
    void LoadPlugin(QString file);
    int Play();
    int Stop();
    int Stop(int speed_factor);
    int Pause();
    int Pause(int speed_factor);
    int Forward(QSlider *slider);
    int Backward(QSlider *slider);
    int GetPlayerTime();
    int GetPlayerCurrentTime();
    float GetVolume();
    PTime GetPTime();
    PTime GetPTime(QString file);
    PTime GetCurrentPTime();
    int GetState();
    void ChangePosition(int position);
    int SetVolume(int vol);
    int SetBalance(int bal);
    QString GetLastPath();
    QString GetLastFileName();
    void SetLastFileName(QString name);
    void SetLastPath(QString path);
    void LoadFileThread(QString file);
    bool isHTTPS(QString link);
    int GetBufferDataSize();
    int GetDataSize();


};

#endif // PLAYER_H
