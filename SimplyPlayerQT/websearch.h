#ifndef WEBSEARCH_H
#define WEBSEARCH_H

#include <QDialog>
#include "HTTP_Downloader.h"
#include "simplyplayer.h"
#include "qyoutubeparser.h"

struct YOUTUBE_DATA
{
    QStringList titles;
    QStringList times;

};


namespace Ui {
class WebSearch;
}

class WebSearch : public QDialog
{
    Q_OBJECT
    
public:
    explicit WebSearch(QWidget *parent = 0);
    QString ConvertWrzutaAddress(QString url);
    ~WebSearch();
    QStringList extractLinks(QString content);
    QString extractName(QString content);
    int getLength(QString content);
    
private slots:
    void on_search_clicked();

    void on_addtoplaylist_clicked();

    void on_download_clicked();

private:
    Ui::WebSearch *ui;
    QStringList columnNames;
};

#endif // WEBSEARCH_H
