#include "imageviewer3.h"
#include <QHBoxLayout>
#include <QMatrix>
#include <QScrollBar>
#include <QGraphicsSceneHoverEvent>
#include "IPLMatrix.h"
#include <QDebug>
#include <QFileInfo>

ImageViewer3::ImageViewer3(MainWindow* imageViewer, QWidget *parent) :
    QFrame(parent)
{
    setLayout(new QHBoxLayout(this));
    setAcceptDrops(true);


    _imageViewerWindow = imageViewer;

    //delete on close
    setAttribute(Qt::WA_DeleteOnClose);

    _image = NULL;
    _rawData = NULL;
    _rawImage = NULL;
    _tabIndex = -1;
    _resultIndex = 0;

    _zoomFitMode = false;

    _scale = 1.0;
    _offsetX = 0;
    _offsetY = 0;

    // add scene
    _pixmapItem = new IPPixmapItem(this);
    _pixmapItem->setAcceptDrops(true);
    _graphicsScene = new QGraphicsScene(this);
    _graphicsScene->addItem(_pixmapItem);

    // add view
    _graphicsView = new IPImageViewerGraphicsView(this);
    _graphicsView->setImageView(this);
    _graphicsView->setScene(_graphicsScene);
    _graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    _graphicsView->setAcceptDrops(true);
    _graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    _graphicsView->setAlignment(Qt::AlignCenter);

    // prevent scrollbars from catching mouse wheel events
    horizontalScrollBar()->installEventFilter(this);
    verticalScrollBar()->installEventFilter(this);


    layout()->addWidget(_graphicsView);

//    setMouseTracking(true);
//    _graphicsView->setMouseTracking(true);

    connect(_graphicsView->horizontalScrollBar(), &QAbstractSlider::sliderMoved, _imageViewerWindow, &MainWindow::on_horizontalScrollBarChanged);
    connect(_graphicsView->verticalScrollBar(), &QAbstractSlider::sliderMoved, _imageViewerWindow, &MainWindow::on_verticalScrollBarChanged);
}

ImageViewer3::~ImageViewer3()
{
    disconnect(_graphicsView->horizontalScrollBar(), &QAbstractSlider::valueChanged, _imageViewerWindow, &MainWindow::on_horizontalScrollBarChanged);
    disconnect(_graphicsView->verticalScrollBar(), &QAbstractSlider::valueChanged, _imageViewerWindow, &MainWindow::on_verticalScrollBarChanged);
//    delete _image;
}

//void ImageViewer3::updateMousePosition(int x, int y)
//{
//    emit mousePositionChanged(x, y);
//}

//void ImageViewer3::on_mouseClicked()
//{
//    emit mouseClicked();
//}


//void ImageViewer3::on_mouseDoubleClicked()
//{
//    emit mouseDoubleClicked();
//}


void ImageViewer3::zoomTo(float scale)
{
    _zoomFitMode = false;

    if(_scale >= 9.0)
        return;

    _scale = scale;

    QMatrix matrix;
    matrix.scale(_scale, _scale);
    _graphicsView->setMatrix(matrix);
}


void ImageViewer3::zoomIn()
{
    _zoomFitMode = false;

    if(_scale >= 9.0)
        return;

    _scale *= 1.1;

    QMatrix matrix;
    matrix.scale(_scale, _scale);
    _graphicsView->setMatrix(matrix);
}

void ImageViewer3::zoomOut()
{
    _zoomFitMode = false;

    if(_scale <= 0.1)
        return;

    _scale *= (1/1.1);

    QMatrix matrix;
    matrix.scale(_scale, _scale);
    _graphicsView->setMatrix(matrix);
}

void ImageViewer3::zoomFit()
{
    _zoomFitMode = true;

    _graphicsView->fitInView(_pixmapItem->boundingRect(), Qt::KeepAspectRatio);

    _scale = _graphicsView->matrix().m11();
}

void ImageViewer3::zoomReset()
{
    _zoomFitMode = false;

    _scale = 1.0;

    QMatrix matrix;
    matrix.scale(_scale, _scale);
    _graphicsView->setMatrix(matrix);
}

bool ImageViewer3::eventFilter(QObject *object, QEvent *event)
{
//    qDebug()<< event->type();
    if (event->type() == QEvent::Wheel)
    {
        return true; // filter the original event out
    }
    return false; // pass other events
}

void ImageViewer3::showEvent(QShowEvent *)
{
    //updateImage();

    if(_zoomFitMode)
    {
        zoomFit();
        emit zoomChanged(zoomFactor());
    }
}

void ImageViewer3::resizeEvent(QResizeEvent *)
{
    if(_zoomFitMode)
    {
        zoomFit();
        emit zoomChanged(zoomFactor());
    }
}

//void IPImageViewerGraphicsView::mouseReleaseEvent(QMouseEvent* event)
//{
//    ((ImageViewer3*) parent())->on_mouseClicked();

//    QGraphicsView::mouseReleaseEvent(event);
//}

//void IPImageViewerGraphicsView::mouseDoubleClickEvent(QMouseEvent* event)
//{
//    ((ImageViewer3*) parent())->on_mouseDoubleClicked();
//}

//void IPPixmapItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
//{
//    int posX = (int) event->pos().x() + 0.5; // round to next full pixel
//    int posY = (int) event->pos().y() + 0.5; // round to next full pixel

//    _imageViewer->updateMousePosition(posX, posY);
//}

void IPImageViewerGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug()<<"drop event in drag enter event  sartabwidget";
    if (event->mimeData()->hasFormat("image/x-sarimg"))
        event->accept();
    else
        event->ignore();
}

void IPImageViewerGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}


void IPImageViewerGraphicsView::dropEvent(QDropEvent *event)
{
    qDebug()<<"drop events=================sartabwidget";
    if (event->mimeData()->hasFormat("image/x-sarimg")) {
        QByteArray pieceData = event->mimeData()->data("image/x-sarimg");
        QDataStream dataStream(&pieceData, QIODevice::ReadOnly);
        QString filePath;
        dataStream >> filePath;
        qDebug()<<filePath;

        QFileInfo fi = QFileInfo(filePath);
        QString file_name = fi.fileName();
        QMap<QString, ImageViewer3*> *temp_vals = &_imageViewer->get_ImageViewerWindow()->get_ImageViewers1();
        if(temp_vals->contains(file_name))
        {
            return;
        }
        ImageViewer3* imageViewer1 = new ImageViewer3(_imageViewer->get_ImageViewerWindow(), _imageViewer->get_ImageViewerWindow()->get_tabWidget());
        _imageViewer->get_ImageViewerWindow()->get_tabWidget()->addTab(imageViewer1, file_name);
        _imageViewer->get_ImageViewerWindow()->get_tabWidget()->setCurrentWidget(imageViewer1);

        QImageReader reader(filePath);
        reader.setAutoTransform(true);
        QImage image = reader.read();
        imageViewer1->setImage(&image);
        imageViewer1->updateImage();
        temp_vals->insert(file_name, imageViewer1);
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void ImageViewer3::updateImage()
{
    if(_image)
    {
        // we make a copy of the image data in order to prevent read access violations
        QPixmap pixmap = QPixmap::fromImage(_image->copy());
        _pixmapItem->setPixmap(pixmap);
        _graphicsScene->setSceneRect(_pixmapItem->boundingRect());
    }
    else
    {
        delete _image;
        _image = NULL;

        _rawData = NULL;
        _rawImage = NULL;

        // convert from IPLImage
        _rawData = _process->getResultData(_resultIndex);

        // if the result is invalid, abort
        if(!_rawData)
        {
            setVisible(false);
            return;
        }
        else
        {
            setVisible(true);
        }

        if(_rawData->type() == IPL_IMAGE_COLOR
                || _rawData->type() == IPL_IMAGE_GRAYSCALE
                || _rawData->type() == IPL_IMAGE_BW
                || _rawData->type() == IPL_IMAGE_ORIENTED
                )
        {
            _rawImage = _rawData->toImage();

            // show normal image
            _image = new QImage(_rawImage->rgb32(), _rawImage->width(), _rawImage->height(), QImage::Format_RGB32);
        }
        else if(_rawData->type() == IPL_IMAGE_COMPLEX)
        {
            _rawComplexImage = _rawData->toComplexImage();

            // show complex image
            _image = new QImage(_rawComplexImage->rgb32(), _rawComplexImage->width(), _rawComplexImage->height(), QImage::Format_RGB32);
        }
        else if(_rawData->type() == IPL_POINT)
        {
            // show point
            _rawData = _process->getResultData(0);
            _image = new QImage(_rawData->toImage()->rgb32(), _rawData->toImage()->width(), _rawData->toImage()->height(), QImage::Format_RGB32);

            QPainter painter(_image);
            painter.setRenderHint(QPainter::Antialiasing, true);

            IPLPoint* p = _process->getResultData(1)->toPoint();

            QPoint point;
            point.setX(p->x());
            point.setY(p->y());

            QBrush brush(Qt::red);
            painter.setBrush(brush);
            painter.drawEllipse(point, 10, 10);
        }
        else if(_rawData->type() == IPL_MATRIX)
        {
            int cellSize = 30;
            int headerSize = 30;

            IPLMatrix* matrix = _rawData->toMatrix();
            _image = new QImage(matrix->width()*cellSize+2*headerSize, matrix->height()*cellSize+2*headerSize, QImage::Format_RGB32);
            _image->fill(Qt::white);

            QPainter painter(_image);
            painter.setRenderHint(QPainter::Antialiasing, true);

            // header
            QPen pen(Qt::black);
            pen.setWidth(2);
            painter.setPen(pen);

            QString title("%1×%2 Matrix");
            painter.drawText(headerSize+2, headerSize-5, title.arg(matrix->width()).arg(matrix->height()));

            // content
            for(int y=0; y < matrix->height(); y++)
            {
                for(int x=0; x < matrix->width(); x++)
                {
                    QRectF box(x*cellSize+headerSize, y*cellSize+headerSize, cellSize, cellSize);
                    painter.drawText(box, Qt::AlignCenter|Qt::AlignVCenter, QString::number(matrix->get(x,y), 'f', 2));
                    painter.drawRect(box);
                }
            }
        }
    }
}
