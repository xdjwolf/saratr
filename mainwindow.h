#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImageReader>
#include <QLabel>
#include <QScrollArea>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif
#include <QListView>
#include <QSize>
#include "ddqlistwidget.h"
#include <QSplitter>
#include <QTabWidget>
#include "ImageViewer3.h"
#include <QWheelEvent>

namespace Ui {
class MainWindow;
}
class ImageViewer3;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool loadFile(const QString &);
    enum ZoomAction
    {
        ZOOM_NONE,  //!<
        ZOOM_IN,    //!< increase zoom by 10%
        ZOOM_OUT,   //!< decrease zoom by 10%
        ZOOM_FIT,   //!< zoom to fit the image inside the current window
        ZOOM_RESET  //!< zoom to 100%
    };
    void zoomAllViewers(ZoomAction action);
    void zoomCurViewers(ZoomAction action);
    QMap<QString, ImageViewer3*> get_ImageViewers1() {return _imageViewers1;}
    QTabWidget* get_tabWidget() {return tabWidget;}
public slots:
    void tabChanged(int);
    void on_zoomChanged(int);
    void on_horizontalScrollBarChanged(int);
    void on_verticalScrollBarChanged(int);
    void on_mousePositionChanged(int, int);
    void on_mouseClick();
    void on_mouseDoubleClick();
    void tabClose(int);
protected:
    void wheelEvent(QWheelEvent *);
private slots:
    void on_actionOpen_O_triggered();
    void on_actionZoom_In_triggered();
    void on_actionPrint_S_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionFit_Screen_triggered();
    void on_actionNormal_Size_triggered();
    void on_actionAbout_triggered();
private:
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    const int IMAGE_SIZE = 170;
    Ui::MainWindow              *ui;
    QLabel                      *imageLabel;
    QLabel                      *permanent;
    double                      scaleFactor;
    QPixmap                     sarImage;
    DDQListWidget               *piecesList;
    QSplitter                   *h1Splitter;
    QTabWidget                  *tabWidget;
    QMap<QString, ImageViewer3*>   _imageViewers1;
    QMap<int, QString>             _imageIndexs;
    int                            _count;
    double                         _currentZoomFactor;
    ZoomAction                     _currentZoomMode;
    bool                           _ignoreZoomEvents;
#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif
};

#endif // MAINWINDOW_H
