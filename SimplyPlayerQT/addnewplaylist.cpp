#include "addnewplaylist.h"
#include "ui_addnewplaylist.h"


AddNewPlaylist::AddNewPlaylist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewPlaylist)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() | Qt::Dialog);

    QPixmap pix = QPixmap(QDir::toNativeSeparators(PLAYLISTD_PATH+"covers/default.png"));

    QGraphicsScene *scene = new QGraphicsScene(0,0,pix.width(),pix.height());
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->itemsBoundingRect() ,Qt::KeepAspectRatio);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(50,50,50), Qt::SolidPattern));
    ui->graphicsView->show();
    coverpath = "";
    type = 0;

}

AddNewPlaylist::~AddNewPlaylist()
{
    delete ui;
}

void AddNewPlaylist::showW(int state)
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    coverpath = "";

    if(state == 0)
    {
        QPixmap pix = QPixmap(QDir::toNativeSeparators(PLAYLISTD_PATH+"covers/default.png"));
        this->setWindowTitle("Create New Playlist");
        ui->pushButton_2->setText("Create");
        QGraphicsScene *scene = new QGraphicsScene(0,0,pix.width(),pix.height());
        scene->addPixmap(pix);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->fitInView(scene->itemsBoundingRect() ,Qt::KeepAspectRatio);
        ui->graphicsView->setBackgroundBrush(QBrush(QColor(50,50,50), Qt::SolidPattern));
        ui->graphicsView->show();
    }
    else
    {
        this->setWindowTitle("Edit Playlist");
        ui->pushButton_2->setText("Edit");
    }

    type = state;

     this->show();
}

void AddNewPlaylist::showW(QPlaylist *playlist, QTab *tab)
{
    type = 1;
    actual_tab = tab;
    actual_playlist = playlist;
    this->setWindowTitle("Edit Playlist");
    ui->pushButton_2->setText("Edit");

    QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);

    settings.beginGroup("Playlists");
    int count = settings.value("Number", 0).toInt();


    for(int i = 0; i < count; i++)
    {
        if(settings.value("Playlist"+QString::number(i), "").toString() == playlist->GetPlaylistName())
        {
            name = settings.value("Playlist"+QString::number(i), 0).toString();
            path = settings.value("File"+QString::number(i), 0).toString();
            break;
        }
    }
    ui->lineEdit->setText(name);
    settings.endGroup();


    QSettings set(APPLICATION_PATH+path, QSettings::IniFormat);
    set.beginGroup("playlist");

    QString coverpath = set.value("Cover","").toString();
    QPixmap pix = QPixmap(QDir::toNativeSeparators(coverpath));

    if(pix.isNull())
        pix = QPixmap(QDir::toNativeSeparators(PLAYLISTD_PATH+"covers/default.png"));

   // QGraphicsPixmapItem cov;


    QGraphicsScene *scene = new QGraphicsScene(0,0,pix.width(),pix.height());
    //scene->addItem(cov);
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->itemsBoundingRect() ,Qt::KeepAspectRatio);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(50,50,50), Qt::SolidPattern));
    ui->graphicsView->show();

    ui->lineEdit_2->setText(set.value("Description","").toString());

    set.endGroup();

    this->show();

}

void AddNewPlaylist::on_pushButton_2_clicked()
{
    if(type == 0)
    {
        if(ui->lineEdit->text().size() > 0)
        {
            emit addPlaylistSignal(ui->lineEdit->text(),ui->lineEdit_2->text(),path);
            this->close();
        }
        else
        {
            QMessageBox::question(0,"Creating new playlist","Playlist name is empty!",QMessageBox::Ok);
        }
    }
    else
    {
        if(ui->lineEdit->text().size() > 0)
        {
            //emit addPlaylistSignal(ui->lineEdit->text(),ui->lineEdit_2->text(),path);

            QSettings settings(PLAYLISTS_PATH, QSettings::IniFormat);

            settings.beginGroup("Playlists");
            int count = settings.value("Number", 0).toInt();

            for(int i = 0; i < count; i++)
            {
                if(settings.value("Playlist"+QString::number(i), "").toString() == actual_playlist->GetPlaylistName())
                {
                    //name = settings.value("Playlist"+QString::number(i), 0).toString();
                   // path = settings.value("File"+QString::number(i), 0).toString();
                    settings.setValue("Playlist"+QString::number(i),ui->lineEdit->text());
                    bool isAct = false;
                    if(settings.value("LastPlaylist", "").toString() == actual_playlist->GetPlaylistName())
                    {
                        settings.setValue("LastPlaylist",ui->lineEdit->text());
                        isAct = true;
                    }
                    settings.endGroup();
                    actual_playlist->SetPlaylistName(ui->lineEdit->text());
                    actual_tab->SetTabName(ui->lineEdit->text());

                   // qDebug() << APPLICATION_PATH+path;

                    QSettings set(APPLICATION_PATH+path, QSettings::IniFormat);
                    set.beginGroup("playlist");
                    QString des = ui->lineEdit_2->text();
                    set.setValue("Description",des);
                    set.setValue("Cover",coverpath);
                    set.endGroup();
                    emit reloadPlaylist(ui->lineEdit->text(), path, actual_playlist, isAct);

                    //break;

                }
                this->close();

            }

        }
        else
        {
            QMessageBox::question(0,"Edit playlist","Playlist name is empty!",QMessageBox::Ok);
        }




    }

}

void AddNewPlaylist::on_pushButton_clicked()
{
    QString Path = QFileDialog::getOpenFileName(this, tr("Open cover image"),
                                                     "",
                                                     tr("Image files (*.png *.jpg *.jpeg *.bmp *.gif)"));
    if(Path.count() > 1)
    {
        coverpath = Path;
    }
    else
    {
        coverpath = "";
    }

    QPixmap pix = QPixmap(QDir::toNativeSeparators(Path));

    QGraphicsScene *scene = new QGraphicsScene(0,0,pix.width(),pix.height());
    scene->addPixmap(pix);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->itemsBoundingRect() ,Qt::KeepAspectRatio);
    ui->graphicsView->setBackgroundBrush(QBrush(QColor(50,50,50), Qt::SolidPattern));
    ui->graphicsView->show();


}

void AddNewPlaylist::on_pushButton_3_clicked()
{
    //this->deleteLater();
    actual_playlist = 0;
    this->close();
}
