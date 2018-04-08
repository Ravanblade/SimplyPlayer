#ifndef QYOUTUBEPARSER_H
#define QYOUTUBEPARSER_H

#include <QObject>
#include "simplyfunctions.h"
#include "HTTP_Downloader.h"
#include <QApplication>
#include <QTimer>
#include <QProcess>
#include <QDir>

struct YOUTUBE_FILE
{
    QString fileURL;
    QString signature;
    QString itag;
    QString quality;

};

struct YOUTUBE_SEARCH
{
    QStringList url;
    QStringList title;
    QStringList time;
};


class QYouTubeParser : public QObject
{
    Q_OBJECT

    public:
        QYouTubeParser(QString link);
        QYouTubeParser();

        QList<YOUTUBE_FILE> getFileLinks();
        QString getTitle();
        QString getTitle(QString link);
        int getLength();
        int getLength(QString link);
        void parseLink(QString link);
        bool parseData();
        YOUTUBE_SEARCH extractSearchedLinks();
        bool validateLink(QString link);
        QString itagToQuality(QString itag);
        QStringList getYouTubeVideo( const QString &data);
        QString fromU( QString s );
        QString Reverse( const QString &str, int from, int to );
        QString unescape( const QString &str );
        QTimer *parserTimer;
        QProcess *proc;

public slots:
        void isDownloaded();
        void timerEvent();

    signals:
        void isParsed();
    //    void parsed(QYouTubeParser*p);


    private:
        void downloadPageData(QString link);
        QString decryptSignature(QString sig);
        bool validateItag(QString itag);
        QString page;
        QString fileURL;
        QString title;
        QString length;
        FileDownloader *yout;
        QList<YOUTUBE_FILE> files;
        int time;
        QString pageContentV;
        QString decodeScripts;
        bool contentDownloaded;
        int counter;


};

#endif // QYOUTUBEPARSER_H
