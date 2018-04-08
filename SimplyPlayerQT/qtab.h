#ifndef QTAB_H
#define QTAB_H

#include <QWidget>

class QTab : public QWidget
{
    Q_OBJECT
public:
    explicit QTab(QWidget *parent = 0);
    QString GetTabName();
    void SetTabName(QString name);
    bool IsActiveTab();
    void SetActiveTab(bool name);

signals:
    
public slots:
    
private:
    QString TabName;
    bool isActiveTab;

protected:
   // void paintEvent(QPaintEvent *e);

};

#endif // QTAB_H
