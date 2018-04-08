#include "simplyfunctions.h"


namespace SimplyFunctions
{

float rand_FloatRange(float a, float b)
{
    return ((b-a)*((float)rand()/RAND_MAX))+a;
}


void SendSignal(QString signalName, int type, int argCount, ...)
{

    va_list arguments;
    va_start ( arguments, argCount );

    QSettings s(SIGNALS_PATH, QSettings::IniFormat);
    s.beginGroup("Signal");
    s.setValue("Name",signalName);

    for(int x = 0; x < argCount; x++)
    {
        if(type == 0)
            s.setValue("Arg" + QString::number(x),QString(va_arg(arguments, int)));
        else if(type == 1)
            s.setValue("Arg" + QString::number(x),QString(va_arg(arguments, char*)));
    }

    s.endGroup();
    va_end(arguments);
}

QString GetFileContent(QString path)
{
    QString content;
    QFile qss(path);
    qss.open(QFile::ReadOnly);
    content = qss.readAll();
    qss.close();

    //PARSE VARIABLES IN QSS
    content.replace("ApplicationPath",QApplication::applicationDirPath());

    //

    return content;
}

bool isHTTP(QString link)
{
    QStringList spl = link.split("://");

    return (spl[0] == "http") || (spl[0] == "https");
}

bool isRTSP(QString link)
{
    QStringList spl = link.split("://");

    return (spl[0] == "rtsp") || (spl[0] == "rtmp");
}

bool isRadio(QString link)
{
    QStringList spl = link.split(":");

    return (spl[0] == "radio");
}

bool isWrzutaAd(QString link)
{
    return link.indexOf(".wrzuta.pl") > 0;
}

bool isYoutubeAd(QString link)
{
    return link.indexOf("youtube.com") > 0;
}

int getMinimum(int *values, int size)
{
    int min;
    for(int j = 0; j < size; j++)
    {
        if(values[j] > 0)
        {
            min = values[j];
            break;
        }

    }

    for (int i = 1; i < size; i++)
        if ((values[i] < min) && (values[i] > 0))
            min = values[i];
    return min;
}

double getMinimum(double *values, int size)
{
    double min;

    for(int j = 0; j < size; j++)
    {
        if(values[j] > 0)
        {
            min = values[j];
            break;
        }

    }

    for (int i = 1; i < size; i++)
        if ((values[i] < min) && (values[i] > 0))
            min = values[i];
    return min;
}

int getMaximum(int *values, int size)
{
    int max = values[0];
    for (int i = 1; i < size; i++)
        if (values[i] < max)
            max = values[i];
    return max;
}

QString section(QString source,int first, int second)
{
    QString d;
    //qDebug() << "First: " << first << "Second: " << second;
    //qDebug() << "*--------SECTION-------*";
    if(first > second)
    {
       // qDebug() << "Szczoteczke w dupe" << first << " " << second;

        for(int i = first; i >= second; i--)
        {
            //qDebug() << i << " = " << source[i];
            d += source[i];
        }
          //  qDebug() << "Juz nie w dupie";
    }
    else
    {
  //  qDebug() << "Szczoteczke w dup123e"<< first << " " << second;
        for(int i = first; i <= second; i++)
        {
           // qDebug() << i << " = " << source[i];
            d += source[i];
        }
    }
    //qDebug() << "*--------END OF SECTION-------*";
    return d;

}



QStringList findFiles(const QString& path = QString())
{
  QStringList files;

  QDir dir = QDir::current();
  if(!path.isEmpty())
    dir = QDir(path);

  dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
#if QT_VERSION >= 0x040000
  QFileInfoList list = dir.entryInfoList();
  for (int i = 0; i < list.size(); ++i)
  {
    QFileInfo fileInfo = list.at(i);
    files.append(dir.absoluteFilePath(fileInfo.fileName()));
  }
#else
  const QFileInfoList* list = dir.entryInfoList();
  if(list)
  {
    QFileInfoListIterator it( *list );
    QFileInfo * fi;
    while( (fi=it.current()) != 0 )
    {
      ++it;
      files.append(dir.absFilePath(fi->fileName()));
    }
  }
#endif

  return files;
}

}


