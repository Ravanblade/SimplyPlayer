#ifndef QPLAYLIST_H
#define QPLAYLIST_H

#include "simplymediaplayer.h"
#include "loadingscreen.h"
#include <QMainWindow>
#include <QApplication>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QTimer>
#include <QMouseEvent>
#include <QUrl>
#include <QDebug>
#include <QVariant>
#include <QList>
#include <QSettings>
#include <windows.h>


//#include <QTreeView>

class QPlaylist : public QTreeWidget
{
    Q_OBJECT

public:
    QPlaylist(QWidget *parent = 0);
    ~QPlaylist(){}
    void RefreshPlaylist(bool RefreshDelegates = false);
    void AddFileToPlaylist(QString Path);
    bool IsSupported(QString format);
    void SavePLS(QString path, QString path2, QStringList list);
    QString GetPlaylistName();
    void SetPlaylistName(QString name);
    QString GetPlaylistPath();
    void SetPlaylistPath(QString path);
    void AddEntryToPlaylistFile(QString playlist_name, QString playlist_file);
    int GetPlaylistRepeatIndex();
    void SetPlaylistRepeatIndex(int index);
    void SetPlayer(SimplyMediaPlayer *pl);

protected:
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent *k);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dragLeaveEvent(QDragLeaveEvent* event);
    void contextMenuEvent(QContextMenuEvent * event);


private:
    bool dropOnCheck(QDropEvent *event, int *dropRow, QModelIndex *dropIndex);
    QString OneToTwo(int Org);
    QString PlaylistName, PlaylistPath;
    int PlaylistRepeatIndex;
    SimplyMediaPlayer *Player;


};


#endif // QPLAYLIST_H
