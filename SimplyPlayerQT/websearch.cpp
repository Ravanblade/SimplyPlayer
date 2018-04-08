#include "websearch.h"
#include "ui_websearch.h"
#include "simplyfunctions.h"


extern WrzutaData *WrzutaD;

WebSearch::WebSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebSearch)
{
    ui->setupUi(this);
    ui->resultView->setColumnHidden(3,true);
    ui->resultView->setColumnWidth(0,200);
    ui->resultView->setColumnWidth(1,50);
    ui->resultView->setColumnWidth(2,50);
    columnNames << "Song Name" << "Length" << "File Size" << "URL";
}

WebSearch::~WebSearch()
{
    delete ui;
}

QString WebSearch::ConvertWrzutaAddress(QString url)
{

    QStringList split = url.split("/");

    QStringList split2 = split[2].split("\\.");

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    int randomValue = qrand() % ((999999 + 1) - 0) + 0;

    QString url2 = "http://" + split2[0] + "/xml/plik/" + split[4] + "/wrzuta.pl/pl/" + QString::number(randomValue);

    qDebug() << url2;

    return url2;


}

void WebSearch::on_search_clicked()
{
    ui->resultView->setRowCount(0);
    ui->resultView->clear();
    ui->resultView->setHorizontalHeaderLabels(columnNames);
    ui->resultView->scrollToTop();
    QString txt = ui->Name->text();
    txt.replace( ' ', '+' );
    //txt = QUrl::toPercentEncoding( txt );
    QString address;

    if(ui->service->currentIndex() == 0)
    {

  /*  address = "http://www.wrzuta.pl/szukaj/audio/" + txt + "/";
    short int pageLimit = ui->limit->value();

    for(int c = 1; c < pageLimit+1; c++)
    {

            address = "http://www.wrzuta.pl/szukaj/audio/" + txt + "/" + QString::number(c);
            qDebug() << address;

         QStringList lst;// =  WrzutaD->Split(QString(wrzuta->downloadedData()));
       // qDebug() << lst;
        if(lst[0].toInt() < pageLimit)
            pageLimit = lst[0].toInt();

        int rowCount = ui->resultView->rowCount();
       // if(ui->resultView->rowCount())

        qDebug() << "List count: " << lst.count();
        ui->resultView->setRowCount(ui->resultView->rowCount()+lst.count());

        for(int t = 1; t < lst.count(); t++)
        {

            lst[t] = ConvertWrzutaAddress(lst[t]);
            qDebug() << lst[t];
        }


        QTableWidgetItem *tmpItem;
        // NAME, FSIZE, FURL, PURL, USERNAME, LENGTH, IMAGE
        for(int i = 1; i < lst.count(); i++)
        {

             QString content;// = wrzuta2->downloadedData();

            tmpItem = new QTableWidgetItem(WrzutaD->getData(content,NAME));
            ui->resultView->setItem( i-1+rowCount, 0, tmpItem);

            tmpItem = new QTableWidgetItem(WrzutaD->getData(content,LENGTH));
            ui->resultView->setItem( i-1+rowCount, 1, tmpItem);

            tmpItem = new QTableWidgetItem(QString::number(WrzutaD->getData(content,FSIZE).toFloat()/(1024*1024))+" MB");
            ui->resultView->setItem( i-1+rowCount, 2, tmpItem);

            tmpItem = new QTableWidgetItem(WrzutaD->getData(content,FURL));
            ui->resultView->setItem( i-1+rowCount, 3, tmpItem);


        }
    }*/

    }
    if(ui->service->currentIndex() == 0)
    {
       // address = "http://www.youtube.com/results?search_query=" + txt;
        short int pageLimit = ui->limit->value();


        for(int c = 1; c < pageLimit+1; c++)
        {

          //  address = "http://www.youtube.com/results?search_query=" + txt + "&page=" + QString::number(c);
            address = "http://gdata.youtube.com/feeds/api/videos?max-results=" + QString::number(c*20) +"&start-index=1&q="+ txt;

          QYouTubeParser *parser = new QYouTubeParser(address);

          QEventLoop loop;
          connect(parser, SIGNAL(isParsed()), &loop, SLOT(quit()));
          loop.exec();


          YOUTUBE_SEARCH lst = parser->extractSearchedLinks();



            int rowCount = ui->resultView->rowCount();

           /* qDebug() << "List count: " << lst.url.count();
            qDebug() << lst.url;
            qDebug() << lst.title;
            qDebug() << lst.time;*/
            ui->resultView->setRowCount(ui->resultView->rowCount()+lst.url.count());


            QTableWidgetItem *tmpItem;

            for(int i = 0; i < lst.url.count(); i++)
            {
              //  qDebug() << i << ": " << lst.title[i] << " " << lst.url[i];
                QString tmp_title = lst.title[i];
                tmp_title.replace("&amp;","&");
                tmpItem = new QTableWidgetItem(tmp_title);
                ui->resultView->setItem( i+rowCount, 0, tmpItem);

                float time = lst.time[i].toFloat();
                int minutes = time/60;
                int seconds = time-(minutes*60);
                QString formatedTime =  QString::number(minutes) + ":" + (seconds > 9 ? QString::number(seconds) : "0" + QString::number(seconds));

                tmpItem = new QTableWidgetItem(formatedTime);
                ui->resultView->setItem( i+rowCount, 1, tmpItem);

                tmpItem = new QTableWidgetItem(lst.url[i]);
                ui->resultView->setItem( i+rowCount, 3, tmpItem);


            }
        }


    }
}

QStringList WebSearch::extractLinks(QString content)
{

        QStringList list;
        QString time;
        QString name;

        QString content2;
        while(1)
        {
            int pos = content.indexOf("<a href=\"/watch?v");

            if(pos < 0)
                break;

            content.remove(0, pos+9);

            pos = content.indexOf("\"",8);
            //qDebug() << pos;
            if(pos < 0)
                break;

            QString tmp = content;

            tmp.remove( pos, tmp.size() - pos );

            list.append("http://www.youtube.com" + tmp);
            //  qDebug() << list;

            content.remove(0, pos);
        }
/*
        while(1)
        {
            int pos = content2.indexOf("<td class=\"watch-card-data-col\">");
            if(pos < 0)
                break;

            content2.remove(0, pos+32);
            pos = content2.indexOf("</td>",0);

            if(pos < 0)
                break;

            QString tmp = content2;
            tmp.remove( pos, tmp.size() - pos );
            time.append(tmp);
            content2.remove(0, pos);
        }

        while(1)
        {
            int pos = content3.indexOf("data-context-item-time=\"");
            if(pos < 0)
                break;

            content3.remove(0, pos+24);
            pos = content3.indexOf("\"",0);

            if(pos < 0)
                break;

            QString tmp = content3;
            tmp.remove( pos, tmp.size() - pos );
            time.append(tmp);
            content3.remove(0, pos);
        }

        while(1)
        {
            int pos = content3.indexOf("<td class=\"watch-card-main-col\" title=\"");
            if(pos < 0)
                break;

            content3.remove(0, pos+24);
            pos = content3.indexOf("\"",0);

            if(pos < 0)
                break;

            QString tmp = content3;
            tmp.remove( pos, tmp.size() - pos );
            time.append(tmp);
            content3.remove(0, pos);
        }
*/






        return list;
}

QString WebSearch::extractName(QString content)
{
/*    QString data = content;

        int ytplayerIdx = data.indexOf( "ytplayer.config" );
        if ( ytplayerIdx > -1 )
        {
            int titleIdx = data.indexOf( "title\": \"", ytplayerIdx );
            if ( titleIdx > -1 )
            {
                int titleEndIdx = data.indexOf( '"', titleIdx += 9 );
                if ( titleEndIdx > -1 )
                    return fromU( data.mid( titleIdx, titleEndIdx - titleIdx ));
            }
        }*/
}



void WebSearch::on_addtoplaylist_clicked()
{
    if(ui->resultView->selectedItems().count() > 0)
    {
        int row = ui->resultView->currentRow();


        QByteArray   bytes  =  ui->resultView->item(row,0)->text().toLatin1();
        const char * name    = bytes.data();

        QByteArray   bytes2  =  ui->resultView->item(row,3)->text().toLatin1();
        const char * url    = bytes2.data();

        QByteArray   bytes3  =  ui->resultView->item(row,1)->text().toLatin1();
        const char * time    = bytes3.data();

        SimplyFunctions::SendSignal("AddFileToCurrentPlaylist", CHARACTER, 3, url, time, name);


    }


}

void WebSearch::on_download_clicked()
{

}
