#ifndef SIMPLYEXPANDER_H
#define SIMPLYEXPANDER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QFrame>

class SimplyExpander : public QWidget
{
    Q_OBJECT
public:
    explicit SimplyExpander(QWidget *parent = 0, QString text = "Album info");
    void setExapndDimensions(int w, int h);
    void addWidget(QWidget *wid);
    void expand();
    void collapse();
    bool isExpanded();
    void setLabelText(QString text);

signals:
    void expandClicked();
    void collapseClicked();

public slots:
    void buttonSlot();

private:
    QPushButton *expander;
    QLabel *label;
    QFrame *widget;
    QFrame *frame;
    int e_w;
    int e_h;
    bool isExpand;
    int tempW;

    
};

#endif // SIMPLYEXPANDER_H
