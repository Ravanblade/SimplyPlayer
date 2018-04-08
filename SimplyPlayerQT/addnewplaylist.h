#ifndef ADDNEWPLAYLIST_H
#define ADDNEWPLAYLIST_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsObject>
#include "qplaylist.h"
#include "qtab.h"

namespace Ui {
class AddNewPlaylist;
}

class AddNewPlaylist : public QWidget
{
    Q_OBJECT

public:
    explicit AddNewPlaylist(QWidget *parent = 0);
    ~AddNewPlaylist();
    void showW(int type);
    void showW(QPlaylist *playlist, QTab *tab);

private:
    Ui::AddNewPlaylist *ui;
    QString coverpath;
    int type;
    QString name,path;
    QPlaylist *actual_playlist;
    QTab *actual_tab;


signals:
    void addPlaylistSignal(QString name, QString author, QString coverpath);
    void reloadPlaylist(QString name, QString path, QPlaylist *playlist, bool isAct);

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
};

#endif // ADDNEWPLAYLIST_H
