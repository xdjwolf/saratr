#ifndef IMAGEVIEWER3_H
#define IMAGEVIEWER3_H
#include <QFrame>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "mainwindow.h"
#include "IPLImage.h"
#include "IPLData.h"
#include "IPLComplexImage.h"
#include "IPLProcess.h"
#include <QAbstractItemView>
#include <QString>

class ImageViewer3;
class MainWindow;

class IPImageViewerGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    IPImageViewerGraphicsView(QWidget * parent = 0) : QGraphicsView(parent)
    {
//        setMouseTracking(true);
        setAcceptDrops(true);
    }
//    virtual void mouseReleaseEvent(QMouseEvent*) override;
//    virtual void mouseDoubleClickEvent(QMouseEvent *) override;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void setImageView(ImageViewer3* img) {_imageViewer = img;}
private:
    ImageViewer3* _imageViewer;
};

//-----------------------------------------------------------------------------
//!Custom QGraphicsPixmapItem for handling mouse events
class IPPixmapItem : public QGraphicsPixmapItem
{
public:
    IPPixmapItem(ImageViewer3* imageViewer, QGraphicsItem * parent = 0) : QGraphicsPixmapItem(parent)
    {
        _imageViewer = imageViewer;
//        setAcceptHoverEvents(true);
        setAcceptDrops(true);
//        setCursor(Qt::CrossCursor);
        setCursor(Qt::ArrowCursor);
    }

    // QGraphicsItem interface
protected:
//    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
private:
    ImageViewer3* _imageViewer;
};


class ImageViewer3:public QFrame
{
    Q_OBJECT
public:
    explicit    ImageViewer3(MainWindow*, QWidget *parent = 0);
    virtual    ~ImageViewer3();
    void updateImage();
    void setTabIndex(int tabIndex)              { _tabIndex = tabIndex; }
    void setResultIndex(int index)              { _resultIndex = index; }
    void setImage(QImage *img)                  {_image=img;}
    int resultIndex()                           { return _resultIndex; }
    int zoomFactor()                            { return _scale*100; }
    QImage*  image()                            { return _image; }
    IPImageViewerGraphicsView getGraphViews() const     { return _graphicsView; }
    MainWindow* get_ImageViewerWindow() {return _imageViewerWindow;}
signals:
    void zoomChanged(int);

//    void mousePositionChanged(int, int);
//    void mouseClicked();
//    void mouseDoubleClicked();

public slots:
//    void on_mouseClicked();
//    void on_mouseDoubleClicked();

private:
    QImage*             _image;
    IPLImage*           _rawImage;
    IPLComplexImage*    _rawComplexImage;
    IPLData*            _rawData;
    QPixmap             _pixmap;
    IPPixmapItem*       _pixmapItem;
    IPLProcess*         _process;
    IPImageViewerGraphicsView*      _graphicsView;
    QGraphicsScene*     _graphicsScene;
    int                 _resultIndex;
    int                 _tabIndex;
    MainWindow*         _imageViewerWindow;
    double              _scale;
    int                 _offsetX;
    int                 _offsetY;
    bool                _zoomFitMode;
    int                 _horizontalScrollValue;
    int                 _verticalScrollValue;

public:
    void zoomTo(float scale);
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void zoomReset();

    bool eventFilter(QObject* object, QEvent* event) override;

    QScrollBar* horizontalScrollBar()   { return _graphicsView->horizontalScrollBar(); }
    QScrollBar* verticalScrollBar()     { return _graphicsView->verticalScrollBar(); }

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *) override;
    virtual void resizeEvent(QResizeEvent *) override;
};

#endif // IMAGEVIEWER3_H
