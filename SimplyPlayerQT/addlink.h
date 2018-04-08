#ifndef ADDLINK_H
#define ADDLINK_H

#include <QDialog>

namespace Ui {
class addlink;
}

class addlink : public QDialog
{
    Q_OBJECT
    
public:
    explicit addlink(QWidget *parent = 0);
    ~addlink();
    
private slots:
    void on_add_clicked();

private:
    Ui::addlink *ui;
};

#endif // ADDLINK_H
