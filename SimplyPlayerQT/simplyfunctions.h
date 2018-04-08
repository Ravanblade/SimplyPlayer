#ifndef SIMPLYFUNCTIONS_H
#define SIMPLYFUNCTIONS_H
//#include "simplyplayer.h"
#include "simplydefinitions.h"
#include <QSettings>
#include <QApplication>
#include <QDebug>

#ifdef WIN32
#include "shobjidl.h"
#endif

namespace SimplyFunctions
{

   void SendSignal(QString signalName, int type, int argCount, ...);
   QString GetFileContent(QString path);
   bool isHTTP(QString link);
   bool isRadio(QString link);
   bool isWrzutaAd(QString link);
   bool isYoutubeAd(QString link);
   bool isRTSP(QString link);
   QStringList findFiles(const QString& path);
   QString section(QString source,int first, int second);
   float rand_FloatRange(float a, float b);
   double getMinimum(double *values, int size);
   int getMinimum(int *values, int size);
   int getMaximum(int *values, int size);

   #ifdef WIN32
   HRESULT AddThumbarButtons(HWND hwnd);
   #endif
}

#endif // SIMPLYFUNCTIONS_H
