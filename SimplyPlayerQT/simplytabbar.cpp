#include "simplytabbar.h"
#include "qtab.h"
#define PLAYLISTS_PATH "data//playlists//playlists.ini"
#define STYLES_PATH "data//temp//stylesheets//"
#include "simplyfunctions.h"

SimplyTabBar::SimplyTabBar(QWidget *parent) :
    QTabBar(parent)
{
   // this->setShape(QTabBar::RoundedNorth);
}

void SimplyTabBar::setParentWidget(QTabWidget *wd)
{
    parentWidget = wd;
}

void SimplyTabBar::mouseDoubleClickEvent(QMouseEvent *e)
 {

    QPoint pos = e->pos();

    if ((this->tabAt(pos) != -1) && (this->tabAt(pos) <=  parentWidget->count()) )
    {

        QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);
        settings.beginGroup("Playlists");

        QString last = settings.value("LastPlaylist", "").toString();

        if(settings.value("LastPlaylist", "").toString() == "")
        {
            if(settings.value("Playlist0", "Main Playlist").toString() != "")
            {
                settings.setValue("LastPlaylist",settings.value("Playlist0", "Main Playlist").toString());
            }
            else
            {
                QMessageBox::about(0,tr("Error"),tr("Error in playlist.ini. There is no playlists!"));
                return;
            }

        }

        if(this->tabAt(pos) > -1 && this->tabAt(pos) <= parentWidget->count())
        {
            parentWidget->setCurrentIndex(this->tabAt(pos));
            QList<QLabel *> label = this->findChildren<QLabel *>();
            for(int j = 0; j < label.count(); j++)
            {
                if(label[j]->text() == last)
                   label[j]->setText("");

            }

            QList<QTab *> tbw = parentWidget->findChildren<QTab *>();
            int index = 0;
            for(int i = 0; i < tbw.count(); i++)
            {
                //QMessageBox::about(0,tr("16"),QString::number(i));
                if(tbw[i]->GetTabName() == settings.value("LastPlaylist", 0).toString())
                {
                    qDebug() << tbw[i]->GetTabName();
                  //  tbw[i]->
                   // parentWidget-
                    index = parentWidget->indexOf(tbw[i]);
                    qDebug() <<index;
                    break;
                }

            }
            this->setTabButton(index,QTabBar::LeftSide,0);
            this->setTabText(index,last);
            //QLabel *lab = label[0];
            //delete label[0];
            QLabel *label2 = new QLabel();
            label2->setText(tr("    ") + this->tabText(parentWidget->currentIndex()));
            label2->setStyleSheet(SimplyFunctions::GetFileContent(QString(STYLES_PATH)+QString("ActiveTab.qss")));
            setTabButton(parentWidget->currentIndex(), QTabBar::LeftSide,((QWidget*)(label2)));
            settings.setValue("LastPlaylist",this->tabText(parentWidget->currentIndex()));
            tabButton(parentWidget->currentIndex(),QTabBar::LeftSide)->move(tabButton(parentWidget->currentIndex(),QTabBar::LeftSide)->geometry().x(),tabButton(parentWidget->currentIndex(),QTabBar::LeftSide)->geometry().y()-2);


            QByteArray   bytes1  =  tabText(parentWidget->currentIndex()).toLatin1();
            const char * ptr1    = bytes1.data();

            SimplyFunctions::SendSignal("SetActivePlaylist", 1,1, ptr1);
            setTabText(parentWidget->currentIndex(),"");


        }




    }
    QTabBar::mouseDoubleClickEvent(e);

}
