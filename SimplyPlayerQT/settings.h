#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>

//#include "spectrumanalyzer.h"

namespace Ui {
class PlayerSettings;
}

class PlayerSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlayerSettings(QWidget *parent = 0);
    ~PlayerSettings();
    void LoadINI();
    
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_brickthick_valueChanged(int arg1);

private:
    Ui::PlayerSettings *ui;
    float gk, gs;
    float odstepy;
    float h_gk;
    bool grid;
    bool kostka;
    bool reflect;
    bool mirror;
    bool upToDown;
    bool waves;
    QLinearGradient kolor_tla;
    int licznik;
    QLinearGradient gradient_1;
    //SpectrumAnalyzer *spec;
};

#endif // SETTINGS_H
