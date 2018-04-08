#include "simplyplayer.h"

void SimplyWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
    //emit changed(event->mimeData());
}

void SimplyWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void SimplyWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

   /* if (mimeData->hasImage()) {
        setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    } else if (mimeData->hasHtml()) {
        setText(mimeData->html());
        setTextFormat(Qt::RichText);
    } else if (mimeData->hasText()) {
        setText(mimeData->text());
        setTextFormat(Qt::PlainText);
    } else if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        QString text;
        for (int i = 0; i < urlList.size() && i < 32; ++i) {
            QString url = urlList.at(i).path();
            text += url + QString("\n");
        }
        setText(text);
    } else {
        setText(tr("Cannot display data"));
    }
*/
    event->acceptProposedAction();
}

void SimplyWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    //clear();
    event->accept();
}
