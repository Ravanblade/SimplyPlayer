#ifndef SIMPLYSETTINGS_H
#define SIMPLYSETTINGS_H

#include <QWidget>
#include <QSettings>
#include <QFileInfo>


namespace Ui {
class SimplySettings;
}

class SimplySettings : public QWidget
{
    Q_OBJECT

public:
    explicit SimplySettings(QWidget *parent = 0);
    ~SimplySettings();
    void LoadINI();
    void setTopBarHeight(int height);
    void LoadSkins();

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_Pick_3_clicked();

private:
    Ui::SimplySettings *ui;
    QColor backgroundCl;
    QList <QColor> color_list;

};

#endif // SIMPLYSETTINGS_H
