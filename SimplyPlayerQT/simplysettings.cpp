#include "simplysettings.h"
#include "ui_simplysettings.h"
#include "simplydefinitions.h"
#include <QColorDialog>
#include <QProcess>
#include <QDirIterator>

SimplySettings::SimplySettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimplySettings)
{
    ui->setupUi(this);
    LoadSkins();
}

SimplySettings::~SimplySettings()
{
    delete ui;
}

void SimplySettings::setTopBarHeight(int height)
{
    ui->frame->resize(ui->frame->width(),ui->frame->height()-height);
    ui->frame->move(ui->frame->x(),ui->frame->y()+height);
}


void SimplySettings::LoadSkins()
{

    QStringList nameFilter("*.zip");
    QDir directory(QDir::fromNativeSeparators(APPLICATION_PATH + "data//skins//"));
    QStringList files = directory.entryList(nameFilter);


    QString processName = QString(QDir::fromNativeSeparators(APPLICATION_PATH + "data//skins//fastzipper.exe"));
    QProcess *proc;

    for(int i = 0; i < files.count(); i++)
    {
        QStringList arguments;
        arguments << files[i] << QDir::fromNativeSeparators(APPLICATION_PATH + "data//skins//tmp//");
        proc = new QProcess();
        proc->start(processName,arguments);
    }



    QStringList all_dirs;
    all_dirs <<  QDir::fromNativeSeparators(APPLICATION_PATH + "data//skins//tmp//");
    QDirIterator directories( QDir::fromNativeSeparators(APPLICATION_PATH + "data//skins//tmp//"), QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while(directories.hasNext())
    {
        directories.next();
        all_dirs << directories.filePath();
    }

    ui->skinsView->addItems(all_dirs);




}


void SimplySettings::LoadINI()
{
    QSettings s(SPECTRUMCONFIG_PATH, QSettings::IniFormat);

    QFileInfo pathInfo(SPECTRUMCONFIG_PATH);

    s.beginGroup("Spectrum");

    ui->brick_dial->setValue(s.value("BrickThickness",4).toInt()); // grubosc kratki domyslne: gk = 2 min: 2
    ui->bar_dial->setValue(s.value("BarThickness",3).toInt()); // gs = 3 min: 1
    ui->space_dial->setValue(s.value("Spacing",4).toInt()); // domyslne 4 min: 1
    ui->grid_2->setChecked(s.value("Grid",false).toBool());
    ui->brick_2->setChecked(s.value("Bricks",false).toBool());
    ui->reflect_2->setChecked(s.value("Reflections",false).toBool());
    ui->mirror_2->setChecked(s.value("Mirror",false).toBool());
    //ui->s.value("Waves",false).toBool();
    ui->vice_2->setChecked(s.value("FromUpToDown",false).toBool());
    int numOfGradients = s.value("GradientsNumber",2).toInt();
    //QList <QColor> color_list;
    ui->barlist_2->setRowCount(numOfGradients);

    for(int i = 1; i < numOfGradients+1;i++)
    {
        color_list.append(QColor(s.value(QString("Gradient") + QString::number(i) + QString("_R"), 255).toInt(),s.value(QString("Gradient") + QString::number(i) + QString("_G"), 255).toInt(),s.value(QString("Gradient") + QString::number(i) + QString("_B"), 255).toInt(),s.value(QString("Gradient") +QString::number(i) + QString("_A"), 255).toInt()));
        QTableWidgetItem *tmpItem = new QTableWidgetItem(QString::number(color_list[i-1].red()) + QString(", ") +QString::number(color_list[i-1].green()) + ", " + QString::number(color_list[i-1].blue()) + ", " + QString::number(color_list[i-1].alpha()));
        ui->barlist_2->setItem(i-1,0,tmpItem);

    }


   // kolor_tla = QColor(s.value("Background_R", 255).toInt(),s.value("Background_G", 255).toInt(),s.value("Background_B", 255).toInt(),s.value("Background_A", 255).toInt());
    ui->bgcolor_2->setStyleSheet("QWidget{background-color: RGBA(" + s.value("Background_R", "255").toString() + "," + s.value("Background_G", "255").toString() + "," + s.value("Background_B", "255").toString() + "," + s.value("Background_A", "255").toString() + ") }");
  //  s.endGroup();

    backgroundCl = QColor(s.value("Background_R", "255").toInt(), s.value("Background_G", "255").toInt(), s.value("Background_B", "255").toInt(), s.value("Background_A", "255").toInt());
    //ui->widget->getSliderColor(int slider_number);
    s.endGroup();


}

void SimplySettings::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void SimplySettings::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void SimplySettings::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void SimplySettings::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void SimplySettings::on_Pick_3_clicked()
{

    backgroundCl = QColorDialog::getColor(backgroundCl, this);
    ui->bgcolor_2->setStyleSheet("QWidget{background-color: RGBA("+ QString::number(backgroundCl.red()) + "," + QString::number(backgroundCl.green()) + "," + QString::number(backgroundCl.blue()) + "," + QString::number(backgroundCl.alpha()) + ") }");

}






