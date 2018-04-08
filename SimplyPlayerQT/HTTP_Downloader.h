#ifndef HTTP_DOWNLOADER_H
#define HTTP_DOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkInterface>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl imageUrl, QObject *parent = 0);

    virtual ~FileDownloader();

    QByteArray downloadedData() const;
    bool isConnectedToNetwork();

signals:
        void downloaded();

private slots:

    void fileDownloaded(QNetworkReply* pReply);

private:

    QNetworkAccessManager m_WebCtrl;

    QByteArray m_DownloadedData;

};

#endif // FILEDOWNLOADER_H
