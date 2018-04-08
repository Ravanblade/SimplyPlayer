#include "addlink.h"
#include "ui_addlink.h"
#include "simplyfunctions.h"
#include "HTTP_Downloader.h"

addlink::addlink(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addlink)
{
    ui->setupUi(this);
}

addlink::~addlink()
{
    delete ui;
}

void addlink::on_add_clicked()
{
    QString usertext = ui->Link->text();
    //QUrl url = QUrl::fromUserInput(usertext);


    if(SimplyFunctions::isHTTP(usertext))
    {

        /*FileDownloader *dw;
        dw = new FileDownloader(QUrl(usertext));

        QEventLoop loop;
        connect(dw, SIGNAL(error()), &loop, SLOT(quit()));
        connect(dw, SIGNAL(downloaded()), &loop, SLOT(quit()));
        loop.exec();

        if(dw->downloadedData().size() > 20)
        {
            QByteArray   bytes  = usertext.toAscii();
            const char * link    = bytes.data();
            SimplyFunctions::SendSignal("AddFileToCurrentPlaylist",1,1,link);
            close();
        }
*/
    }
    else
    {
        ui->Link->setText("http://" + ui->Link->text());
        on_add_clicked();
        //QMessageBox::question(0,"Add link","Invalid link format. ",QMessageBox::Ok);
    }



}

