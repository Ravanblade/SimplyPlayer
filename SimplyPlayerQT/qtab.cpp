#include "qtab.h"
#include <QtWidgets>

QTab::QTab(QWidget *parent) :
    QWidget(parent)
{
    isActiveTab = false;
}

QString QTab::GetTabName()
{
    return TabName;
}

void QTab::SetTabName(QString name)
{
    TabName = name;
}

bool QTab::IsActiveTab()
{
    return isActiveTab;
}

void QTab::SetActiveTab(bool name)
{
    isActiveTab = name;
}
