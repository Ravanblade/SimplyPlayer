#include "settings.h"
#include "simplydefinitions.h"
#include "ui_settings.h"

PlayerSettings::PlayerSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerSettings)
{
    ui->setupUi(this);
    LoadINI();
    this->setWindowFlags(this->windowFlags() | Qt::Dialog);
   //ui->widget_2->setAnimationMode(1);
   // spec = new SpectrumAnalyzer(ui->widget_2);
  //  spec->setGeometry(ui->widget_2->geometry());
   // spec->setAnimationMode(1);
}

PlayerSettings::~PlayerSettings()
{
    delete ui;
}

void PlayerSettings::LoadINI()
{
    QSettings s(SPECTRUMCONFIG_PATH, QSettings::IniFormat);

    QFileInfo pathInfo(SPECTRUMCONFIG_PATH);

    s.beginGroup("Spectrum");

    ui->brickthick->setValue(s.value("BrickThickness",4).toInt()); // grubosc kratki domyslne: gk = 2 min: 2
    ui->barthick->setValue(s.value("BarThickness",3).toInt()); // gs = 3 min: 1
    ui->space->setValue(s.value("Spacing",4).toInt()); // domyslne 4 min: 1
    ui->grid->setChecked(s.value("Grid",false).toBool());
    ui->brick->setChecked(s.value("Bricks",false).toBool());
    ui->reflect->setChecked(s.value("Reflections",false).toBool());
    ui->mirror->setChecked(s.value("Mirror",false).toBool());
    //ui->s.value("Waves",false).toBool();
    ui->vice->setChecked(s.value("FromUpToDown",false).toBool());
    int numOfGradients = s.value("GradientsNumber",2).toInt();
    QList <QColor> color_list;
    ui->barlist->setRowCount(numOfGradients);

    for(int i = 1; i < numOfGradients+1;i++)
    {
        color_list.append(QColor(s.value(QString("Gradient") + QString::number(i) + QString("_R"), 255).toInt(),s.value(QString("Gradient") + QString::number(i) + QString("_G"), 255).toInt(),s.value(QString("Gradient") + QString::number(i) + QString("_B"), 255).toInt(),s.value(QString("Gradient") +QString::number(i) + QString("_A"), 255).toInt()));
        QTableWidgetItem *tmpItem = new QTableWidgetItem(QString::number(color_list[i-1].red()) + QString(", ") +QString::number(color_list[i-1].green()) + ", " + QString::number(color_list[i-1].blue()) + ", " + QString::number(color_list[i-1].alpha()));
        ui->barlist->setItem(i-1,0,tmpItem);

    }


   // kolor_tla = QColor(s.value("Background_R", 255).toInt(),s.value("Background_G", 255).toInt(),s.value("Background_B", 255).toInt(),s.value("Background_A", 255).toInt());
    ui->bgcolor->setStyleSheet("QWidget{background-color: RGBA(" + s.value("Background_R", "255").toString() + s.value("Background_G", "255").toString() + s.value("Background_B", "255").toString() + s.value("Background_A", "255").toString() + ") }");
  //  s.endGroup();


    //ui->widget->getSliderColor(int slider_number);


}

void PlayerSettings::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void PlayerSettings::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void PlayerSettings::on_pushButton_3_clicked()
{
    LoadINI();
    ui->stackedWidget->setCurrentIndex(2);
}

void PlayerSettings::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void PlayerSettings::on_brickthick_valueChanged(int arg1)
{

}
