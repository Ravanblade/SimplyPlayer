#include "qplaylist.h"
#include "simplyplayer.h"
#include <windows.h>
#include "loadingscreen.h"
#include "simplyfunctions.h"
#include "simplymediaplayer.h"
//extern SimplyWindow *SW;

extern SimplyWindow *SimplyWindow;
//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1250"));

QPlaylist::QPlaylist(QWidget *parent) :
    QTreeWidget(parent)
{
    setAcceptDrops(true);
    PlaylistName = "Playlist";
    PlaylistPath = "data//playlists//playlist.pls";
    //this->setItemDelegateForColumn(5, new CloseButton( this ));
    PlaylistRepeatIndex = -1;
    Player = new SimplyMediaPlayer();
}
bool QPlaylist::IsSupported(QString format)
{
    char *formats[5] = {"mp3","wav","ogg"};

    for(int i = 0; i < 5; i++)
    {
      if(format == QString(formats[i]))
      {
         return true;
      }

    }

   return false;
}


int QPlaylist::GetPlaylistRepeatIndex()
{
    return PlaylistRepeatIndex;
}

void QPlaylist::SetPlaylistRepeatIndex(int index)
{
    PlaylistRepeatIndex = index;
}

void QPlaylist::AddFileToPlaylist(QString Path)
{
    QFileInfo pathInfo(Path);
    if (IsSupported(pathInfo.suffix()))
    {
        int count = this->topLevelItemCount() + 1;

        QTreeWidgetItem *Item = new QTreeWidgetItem(0);

        PTime Time = Player->GetPTime(Path);
        Item->setText(0,"");
        Item->setText(1,QString::number(count));
        Item->setText(2,pathInfo.fileName());
        Item->setText(3,Path);
        Item->setText(4,QString::number((int)Time.minutes) + ":" + OneToTwo(Time.seconds));
        this->insertTopLevelItem(this->topLevelItemCount(),Item);
       // this->setItemDelegateForColumn(5, new CloseButton( this ));
        QByteArray   bytes  = this->GetPlaylistName().toLatin1();
        const char * ptr    = bytes.data();

        QByteArray   bytes1  = QString::number(count-1).toLatin1();
        const char * ptr1    = bytes1.data();

        SimplyFunctions::SendSignal("AddDelegatesToPlaylist", CHARACTER, 2, ptr , ptr1);
        //SendSignal("AddDelegatesToPlaylist", CHARACTER, 1, ptr);
        this->RefreshPlaylist();
    }
}

QString QPlaylist::OneToTwo(int Org)
{
  if(Org < 10)
        return "0" + QString::number(Org);
  else
        return QString::number(Org);
}

void QPlaylist::SavePLS(QString path, QString path2, QStringList list)
{
    QSettings settings(path, QSettings::IniFormat);

    QFileInfo pathInfo(path);
//    if(!pathInfo.exists())
//        return INVALID_DIR;

    settings.beginGroup("playlist");
    settings.setValue("NumberOfEntries", list.count());

    for(int i = 0; i < settings.value("NumberOfEntries",0).toInt(); i++)
    {
        QFileInfo pathInfo(list[i]);
        settings.setValue("file"+QString::number(i), path2 + list[i]);
        settings.setValue("title"+QString::number(i), pathInfo.fileName());
    }


    settings.endGroup();
}

QString QPlaylist::GetPlaylistName()
{
    return PlaylistName;
}

void QPlaylist::SetPlaylistName(QString name)
{
    PlaylistName = name;
}

QString QPlaylist::GetPlaylistPath()
{
    return PlaylistPath;
}

void QPlaylist::SetPlaylistPath(QString path)
{
    if(path.left(4) == "data")
        PlaylistPath = QDir::fromNativeSeparators(QApplication::applicationDirPath() + "/" + path);
    else
        PlaylistPath = path;
}


void QPlaylist::AddEntryToPlaylistFile(QString playlist_name, QString playlist_file)
{
    QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);

    QFileInfo pathInfo(PLAYLISTS_PATH);
    //if(!pathInfo.exists())
    //    return INVALID_DIR;

    settings.beginGroup("Playlists");
    int count = settings.value("Number",1).toInt();

    settings.setValue("Number", count+1);
    settings.setValue("Playlist"+QString::number(count), playlist_name);
    settings.setValue("File"+QString::number(count), playlist_file);

    settings.endGroup();
}

void QPlaylist::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if (event->mimeData()->hasFormat("FileNameW"))
    {

        for(int i = 0; i < urls.count(); i++)
        {
            if (urls.isEmpty())
                return;


            QString fileName = urls.at(i).toLocalFile();
            QFileInfo fileInfo(fileName);
            if(fileInfo.isSymLink())
            {
                fileName = fileInfo.symLinkTarget();
                fileInfo.setFile(fileName);
            }

            if(fileInfo.isFile())
            {
                AddFileToPlaylist(fileName);
                RefreshPlaylist();
            }
            else if(fileInfo.isDir())
            {
                LoadingScreen *screen = new LoadingScreen(0,"Loading playlist");
                screen->show();
                QDir myDir(fileName);
               // myDir.entryList()
                QStringList filter;
                filter << "*.mp3" << "*.wav" << "*.ogg";
                QStringList list = myDir.entryList(filter);

                //SavePLS("data//playlists//" + fileInfo.fileName() + ".pls", urls.at(i).toLocalFile() + tr("/"), list);
                SavePLS("data//playlists//" + fileInfo.fileName() + ".pls", fileInfo.filePath() + tr("/"), list);
                AddEntryToPlaylistFile(fileInfo.fileName(), QString("data//playlists//" + fileInfo.fileName() + ".pls"));

                QByteArray   bytes1  = fileInfo.fileName().toLatin1();
                const char * ptr1    = bytes1.data();

                QByteArray   bytes2  = QString("data//playlists//" + fileInfo.fileName() + ".pls").toLatin1();
                const char * ptr2    = bytes2.data();

                SimplyFunctions::SendSignal("CreateNewPlaylist", CHARACTER, 2, ptr1 , ptr2);
                //screen->close();
                delete screen;
                delete ptr1;
                delete ptr2;

               // SavePlaylist();

                //CreateNewPlaylist
            }







       /*    // if (urls.isEmpty())
           //     return;
         QList<QUrl> urls = event->mimeData()->urls();
            QString fileName = urls[i].toLocalFile();
           // if (fileName.isEmpty())
           //     return;

            QSettings settings("temp.txt", QSettings::IniFormat);
            settings.beginGroup("MainWindow");
            settings.setValue("qwe", fileName);
            settings.endGroup();

            AddItemTo(fileName);

            RefreshPlaylist();
*/         /*
              QByteArray data = mimeData->data("FileNameW");
              QString filename = QString::fromUtf16((ushort*)data.data(), data.size() / 2);
              QString fle = filename.remove(filename.size()-1,2);
              fle = fle.replace(QString("\\"),QString("/"));
              AddItemTo(fle);
              RefreshPlaylist();

              QSettings settings("temp.txt", QSettings::IniFormat);
              settings.beginGroup("MainWindow");
              settings.setValue("qwe", fle);
              settings.endGroup();
*/
                }
    }
    else
    {

    int dropRow;
    QModelIndex dropIndex;

    if (dropOnCheck(event, &dropRow, &dropIndex))
    {
        QList<QModelIndex> idxs = selectedIndexes();
        QList<QPersistentModelIndex> indexes;

        for (int i = 0; i < idxs.count(); i++)
        {
            indexes.append(idxs.at(i));
        }

        if (!indexes.contains(dropIndex))
        {

            QPersistentModelIndex dropRowIndex = model()->index(dropRow, 0, dropIndex);

            QList<QTreeWidgetItem *> taken;
            for (int i = indexes.count() - 1; i >= 0; --i)
            {
                QTreeWidgetItem *parent = itemFromIndex(indexes.at(i));

                if (!parent || !parent->parent())
                {
                    taken.append(takeTopLevelItem(indexes.at(i).row()));
                }
                else
                {
                    taken.append(parent->parent()->takeChild(indexes.at(i).row()));
                }
            }

           for (int i = 0; i < indexes.count(); ++i)
            {

                if (dropRow == -1)
                {
                    if (dropIndex.isValid())
                    {
                        QTreeWidgetItem *parent = itemFromIndex(dropIndex);
                        parent->insertChild(parent->childCount(), taken.takeFirst());
                        RefreshPlaylist(true);
                    }
                    else
                    {
                        insertTopLevelItem(topLevelItemCount(), taken.takeFirst());
                        RefreshPlaylist(true);
                    }
                }
                else
                {
                    int r = dropRowIndex.row() >= 0 ? dropRowIndex.row() : dropRow;
                    if (dropIndex.isValid())
                    {
                        QTreeWidgetItem *parent = itemFromIndex(dropIndex);
                        parent->insertChild(qMin(r, parent->childCount()), taken.takeFirst());
                        RefreshPlaylist(true);
                    }
                    else
                    {
                        insertTopLevelItem(qMin(r, topLevelItemCount()), taken.takeFirst());
                        RefreshPlaylist(true);
                    }
                }
            }


            event->setDropAction(Qt::CopyAction);
            event->accept();
            RefreshPlaylist(true);
            return;
        }
    }


    QTreeWidget::dropEvent(event);
    RefreshPlaylist();
   }
}

bool QPlaylist::dropOnCheck(QDropEvent *event, int *dropRow, QModelIndex *dropIndex)
{
    if (event->isAccepted())
        return false;

    QModelIndex index =  indexAt(event->pos());

    if (supportedDropActions() & event->dropAction()) {
        int row = -1;
        //int col = -1;
        if (index != rootIndex() && index.isValid()) {
            QRect rect = visualRect(index);
            qreal criticalY = rect.center().y();

            if (event->pos().y()<=criticalY) {
                row = index.row();
                //col = index.column();
                index = index.parent();
            } else {
                row = index.row() + 1;
                //col = index.column();
                index = index.parent();
            }
            *dropIndex = index;
            *dropRow = row;
            // *dropCol = col;
            RefreshPlaylist(true);
            return true;
        }
    }
    RefreshPlaylist(true);
    return false;
}

void QPlaylist::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

void QPlaylist::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

void QPlaylist::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void QPlaylist::RefreshPlaylist(bool RefreshDelegates)
{
  /*  int items = this->topLevelItemCount();
    int font_size = this->font().pixelSize()/2;
    if(items < 10)
       this->setColumnWidth(1,15);
    else if(items > 9 || items < 100)
        //this->setColumnWidth(1,19);
        this->setColumnWidth(1,19);
    else if(items > 99 || items < 1000)
        //this->setColumnWidth(1,25);
        this->setColumnWidth(1,25);
    else if(items > 999 || items < 10000)
        //this->setColumnWidth(1,29);
        this->setColumnWidth(1,29);
    */

    if(RefreshDelegates)
    {
        for(int j = 0; j < this->topLevelItemCount(); j++)
        {
            //qDebug() << "Finding: " << j;
            if(this->itemWidget(this->topLevelItem(j),6) == 0 || this->itemWidget(this->topLevelItem(j),5) == 0)
            {
                //qDebug() << "Found at:" << j;
                QByteArray   bytes  = this->GetPlaylistName().toLatin1();
                const char * ptr    = bytes.data();
                QByteArray   bytes1  = QString::number(j).toLatin1();
                const char * ptr1    = bytes1.data();

                SimplyFunctions::SendSignal("AddDelegatesToPlaylist", CHARACTER, 2, ptr , ptr1);
            }



        }
    }


    this->resizeColumnToContents(1);
    this->resizeColumnToContents(4);

    for(int i = 0; this->topLevelItemCount() > i; i++)
    {
          this->topLevelItem(i)->setData(1,0,i+1);
    }

    int anv_size = 0;

    for(int j = 0; this->columnCount()-1 > j; j++)
    {
        if(j == 2) continue;
      //  qDebug() << this->columnWidth(j);

        anv_size += this->columnWidth(j);
    }

   // qDebug() << this->parentWidget()->width() << anv_size;
    if(this->verticalScrollBar()->maximum() == this->verticalScrollBar()->minimum())
        this->setColumnWidth(2,this->parentWidget()->width()-anv_size-40);
    else
        this->setColumnWidth(2,this->parentWidget()->width()-anv_size-45);

//qDebug() << "hehe";


/*

    if(this->topLevelItemCount() < 100)
        this->setColumnWidth(2,185);
    else if(this->topLevelItemCount() < 999)
        this->setColumnWidth(2,171);
    else if(this->topLevelItemCount() < 9999)
        this->setColumnWidth(2,167);
*/


}

/*void QPlaylist::itemDoubleClicked( QTreeWidgetItem * item, int column )
{
   // SW->on_Playlist_doubleClicked(item);
//MyGame *theGame = MyGame::getInstance();
on_Playlist_doubleClicked(item);
//SimplyWindow *spw = SimplyWindow::get
}*/


void QPlaylist::keyPressEvent(QKeyEvent *k)
{
    //QMessageBox::about(0,tr("3"),tr("4"));
    //qDebug() << "Key event";

    switch(k->key())
    {

      //qDebug() << "Checking";
        case Qt::Key_Return:
        {

            if(this->selectedItems().count() > 0)
            {


            QString Pt = this->topLevelItem(this->currentIndex().row())->data(3,0).toString();

            if(Pt.size() > 0)
            {
                QByteArray bytes = Pt.toLatin1();
                char *file = bytes.data();
                //SetGlobalIndex(this->topLevelItem(this->currentIndex().row())->data(1,0).toInt(0)-1);
                Player->LoadFile(file);
                Player->Play();
            }

            }

           break;
        }

        case Qt::Key_Delete:
        {
           if(this->selectedItems().count() > 0)
           {
               this->takeTopLevelItem(this->topLevelItem(this->currentIndex().row())->data(1,0).toInt(0)-1);
               RefreshPlaylist();
           }

            break;
        }

        case Qt::Key_Up:
        {

            if(this->currentIndex().row()-1 >= 0)
            {
                 this->currentItem()->setSelected(false);
                 this->setItemSelected(this->itemAbove(this->currentItem()),true);
                 this->setCurrentItem(this->itemAbove(this->currentItem()),0,QItemSelectionModel::NoUpdate);
            }

            break;
        }

        case Qt::Key_Down:
        {

            if(this->currentIndex().row() != this->topLevelItemCount()-1)
            {
                this->currentItem()->setSelected(false);
                this->setItemSelected(this->itemBelow(this->currentItem()),true);
                this->setCurrentItem(this->itemBelow(this->currentItem()),0,QItemSelectionModel::NoUpdate);
            }

            break;
        }

        case Qt::Key_Left:
        {

            Player->ChangePosition(Player->GetPlayerCurrentTime()-5);
            break;
        }

        case Qt::Key_Right:
        {

            Player->ChangePosition(Player->GetPlayerCurrentTime()+5);
            break;
        }


    default:
        break;

    }
   // QTreeWidget::keyPressEvent(k);

}

void QPlaylist::contextMenuEvent (QContextMenuEvent * event)
{

}
