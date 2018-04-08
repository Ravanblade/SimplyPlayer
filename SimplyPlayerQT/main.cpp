#include <QApplication>
#include "simplyplayer.h"
#include <QLibrary>
//;
WrzutaData *WrzutaD;
#include <windows.h>
#include "qskinobject.h"
#include "qtwin.h"
#include "simplyfunctions.h"
#include "simplydefinitions.h"
//--------------------------------

int main(int argc, char *argv[])
{
    putenv(QString("QT_QPA_PLATFORM_PLUGIN_PATH = "+ QDir::fromNativeSeparators(APPLICATION_PATH + "data//platform//")).toStdString().c_str());
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName ("Windows-1250"));
    QApplication::addLibraryPath(QDir::fromNativeSeparators(APPLICATION_PATH + "data//platform//"));
    //QTextCodec::setCodecForTr ();
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName ("Windows-1250"));
    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont(":/fonts/LCD");


    QStringList argu = QCoreApplication::arguments();

    if(argu.count() > 0)
    {
        for(int i = 1; i < argu.count(); i++)
        {
            QByteArray   bytes1  = argu[i].toLatin1();
            const char * ptr1    = bytes1.data();

            SimplyFunctions::SendSignal("PlayFile",1,1,ptr1);
        }
    }


#ifdef WIN32
    TCHAR *MutexName = TEXT("SimplyPlayerMutex");
    HANDLE Mutex = CreateMutex(NULL, FALSE, MutexName);
    if(GetLastError() == ERROR_ALREADY_EXISTS)
        return 0;
#endif




    SimplyWindow w;
    w.show();
   // QCoreApplication::setApplicationName("Simply Player" );
   // QCoreApplication::setOrganizationName("Ravan");
    return a.exec();
}
