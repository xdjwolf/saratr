#include "ddqlistwidget.h"
#include <QMimeData>
#include <QModelIndexList>
#include <QPixmap>
#include <QPoint>
#include <QList>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QListWidget>
#include <QDropEvent>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include <QUrl>
#include <QDragLeaveEvent>
#include <QDrag>
#include <QtGui>

DDQListWidget::DDQListWidget(QListWidget *parent):QListWidget(parent)
{
    setVerticalStepsPerItem(1);
}

DDQListWidget::~DDQListWidget(){}


void DDQListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-sarimg"))
        event->accept();
    else
        event->ignore();
}

void DDQListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-sarimg")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void DDQListWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-sarimg")) {
        QByteArray pieceData = event->mimeData()->data("image/x-sarimg");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QPixmap pixmap;
        QString filePath;
        dataStream >> pixmap >> filePath;
        addPiece(pixmap, filePath);
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void DDQListWidget::addPiece(QPixmap pixmap, QString filePath)
{
    QListWidgetItem *pieceItem = new QListWidgetItem(this);
    pieceItem->setIcon(QIcon(pixmap));
    pieceItem->setData(Qt::UserRole, QVariant(pixmap));
    pieceItem->setData(Qt::UserRole+1, filePath);
    pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
}

void DDQListWidget::startDrag(Qt::DropActions /*supportedActions*/)
{
    QListWidgetItem *item = currentItem();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
//    QPixmap pixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
    QString filePath = item->data(Qt::UserRole+1).toString();

    dataStream << filePath;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("image/x-sarimg", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}
