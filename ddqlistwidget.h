#ifndef DDQLISTWIDGET_H
#define DDQLISTWIDGET_H
#include <QListWidget>
#include <QMimeData>
#include <QModelIndexList>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QListWidget>
#include <QDropEvent>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include <QUrl>
#include <QDragLeaveEvent>

class DDQListWidget : public QListWidget
{
    Q_OBJECT
public:
    DDQListWidget(QListWidget *parent = 0);
    ~DDQListWidget();
    void addPiece(QPixmap pixmap, QString filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void startDrag(Qt::DropActions supportedActions) Q_DECL_OVERRIDE;
private:

};



#endif // DDQLISTWIDGET_H
