#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include "simplyplayer.h"


class CloseButton : public QStyledItemDelegate {
    Q_OBJECT
public:

    explicit CloseButton(QWidget *parent = 0,
                         const QPixmap &closeIcon = QPixmap())
        : QStyledItemDelegate(parent)
        , m_closeIcon(closeIcon)
    {
        if(m_closeIcon.isNull())
        {
            m_closeIcon = qApp->style()
                ->standardPixmap(QStyle::SP_DialogCloseButton);

        }
    }

    QPoint closeIconPos(const QStyleOptionViewItem &option) const
    {
        //return QPoint(option.rect.right() - m_closeIcon.width() - margin,
        //              option.rect.center().y() - m_closeIcon.height()/2);
        return QPoint(option.rect.right() - m_closeIcon.width()-20,
                      option.rect.center().y() - m_closeIcon.height()/2);
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const
    {

        // Only display the close icon for top level items...
    /*    if((!index.parent().isValid())&& (option.state & QStyle::State_MouseOver))
                // ...and when the mouse is hovering the item
                // (mouseTracking must be enabled on the view)

        {
            //index.row()
            painter->drawPixmap(closeIconPos(option), m_closeIcon);
        }

        else */if(!index.parent().isValid())
        {
            painter->drawPixmap(closeIconPos(option), m_closeIcon);
        }

       // QStyledItemDelegate::paint(painter, option, index);

    }

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);

        // Make some room for the close icon
        if(!index.parent().isValid())
        {
            size.rwidth() = 10;//m_closeIcon.width()-10;
            size.setHeight(qMax(size.height(), m_closeIcon.height()-9));
        }
        size.setWidth(10);

        return size;
    }

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index)
    {
        // Emit a signal when the icon is clicked

        //QMessageBox::about(0,tr("65"),tr("54"));
        qDebug() << "event";

/*        if(!index.parent().isValid() && event->type() == QEvent::HoverEnter)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            QRect closeButtonRect = m_closeIcon.rect()
                    .translated(closeIconPos(option));

            if(closeButtonRect.contains(mouseEvent->pos()))
            {
              //  if(!hover)
               // {
                    qDebug() << "refresh";
                    m_closeIcon = QPixmap(QString(SKINIMAGE_PATH) + QString("refresh.png"));
                    QWidget *parent = (QWidget*)this->parent();
                    parent->update();
               //     hover = true;
               // }

            }
        }
        if(!index.parent().isValid() && event->type() == QEvent::HoverLeave)
            {
             //   if(hover)
              //  {
                    qDebug() << "close";
                    m_closeIcon = qApp->style()->standardPixmap(QStyle::SP_DialogCloseButton);
                    QWidget *parent = (QWidget*)this->parent();
                     parent->update();
                //    hover = false;
                //}
            }
        //}
*/
        if(!index.parent().isValid() && event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            QRect closeButtonRect = m_closeIcon.rect()
                    .translated(closeIconPos(option));


            if(closeButtonRect.contains(mouseEvent->pos()))
            {
                emit closeIndexClicked(index.row());
            }
        }



        //return false;
    }



signals:
    void closeIndexClicked(int row);

private:
    QPixmap m_closeIcon;
    static const int margin = 0; // pixels to keep arount the icon
    bool hover;

    Q_DISABLE_COPY(CloseButton)
};
#endif // CLOSEBUTTON_H
