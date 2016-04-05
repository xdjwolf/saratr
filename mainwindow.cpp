#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QStandardPaths>
#include <QStringList>
#include <QDebug>
#include <QScreen>
#include <QPrintDialog>
#include <QPainter>
#include <QScrollBar>
#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#endif
#include <QMessageBox>
#include <QHBoxLayout>
#include <QListView>
#include <QFileInfo>
#include <QDir>
#include <iostream>
#include <QSpacerItem>
#include <QSplitter>
#include <QFileInfo>
#include <QMapIterator>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
//    this->setWindowIcon(QIcon(":/res/logo.ico"));
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    h1Splitter = new QSplitter;
    QSplitter *hSplitter = new QSplitter;
    hSplitter->setOrientation(Qt::Horizontal);
    piecesList = new DDQListWidget;
    piecesList->setFlow(QListView::TopToBottom);
    piecesList->setViewMode(QListView::IconMode);
    piecesList->setMovement(QListView::Static);
    piecesList->setFixedWidth(250);
    piecesList->setDragEnabled(true);
    piecesList->setAcceptDrops(true);
    piecesList->setDropIndicatorShown(true);
    piecesList->setSelectionMode(QAbstractItemView::SingleSelection);
    piecesList->setDragDropMode(QAbstractItemView::InternalMove);
    piecesList->setAlternatingRowColors(true);
    piecesList->setWrapping(false);
    //status bar
    permanent = new QLabel;
    permanent->setFrameStyle(QFrame::NoFrame);
    permanent->setText("www.saratr.com");
    ui->statusBar->addPermanentWidget(permanent);

    tabWidget = new QTabWidget(this);
    tabWidget->setVisible(true);
    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);

    h1Splitter->addWidget(piecesList);
    h1Splitter->addWidget(tabWidget);
    h1Splitter->setStretchFactor(1,1);
    frameLayout->addWidget(h1Splitter);
    setCentralWidget(frame);
    resize(QGuiApplication::primaryScreen()->availableSize());
    _currentZoomMode = ZOOM_NONE;
    _currentZoomFactor = 1.0;
    _ignoreZoomEvents = false;
    _count = 0;
    tabWidget->setTabsClosable(true);
    connect(tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabClose(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFileInfo fi = QFileInfo(fileName);
    QString file_name = fi.fileName();
    if(_imageViewers1.contains(file_name))
    {
        return false;
    }
    ImageViewer3* imageViewer1 = new ImageViewer3(this, tabWidget);
    connect(imageViewer1, &ImageViewer3::zoomChanged, this, &MainWindow::on_zoomChanged);
//    connect(imageViewer1, &ImageViewer3::mousePositionChanged, this, &MainWindow::on_mousePositionChanged);
//    connect(imageViewer1, &ImageViewer3::mouseClicked, this, &MainWindow::on_mouseClick);
//    connect(imageViewer1, &ImageViewer3::mouseDoubleClicked, this, &MainWindow::on_mouseDoubleClick);
    imageViewer1->zoomTo(_currentZoomFactor);
    tabWidget->addTab(imageViewer1, file_name);
    tabWidget->setCurrentWidget(imageViewer1);

    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    QImage image = reader.read();
    imageViewer1->setImage(&image);
    imageViewer1->updateImage();
    _imageViewers1.insert(file_name, imageViewer1);
    QString file_path = fi.absolutePath();

    //list all of the files under the absolute path
    QDir dir(file_path);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    QString file_suffix;
    piecesList->clear();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        file_suffix = fileInfo.completeSuffix();
        if((!QString::compare(file_suffix, "bmp", Qt::CaseInsensitive))||(QString::compare(file_suffix, "jpg", Qt::CaseInsensitive))
                    ||(QString::compare(file_suffix, "jpeg", Qt::CaseInsensitive))){
             QListWidgetItem* lwi = new QListWidgetItem(QIcon(fileInfo.absoluteFilePath()), fileInfo.baseName());
             lwi->setTextAlignment(Qt::AlignCenter);
             lwi->setData(Qt::ToolTipRole, fileInfo.absoluteFilePath());
             lwi->setData(Qt::UserRole+1, fileInfo.absoluteFilePath());
             piecesList->addItem(lwi);
        }
    }
    piecesList->setIconSize(QSize(220,200));
    piecesList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    piecesList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scaleFactor = 1.0;
    ui->actionPrint_S->setEnabled(true);
    ui->actionFit_Screen->setEnabled(true);
    updateActions();
    return true;
}

void MainWindow::updateActions(){
    ui->actionZoom_In->setEnabled(!ui->actionFit_Screen->isChecked());
    ui->actionZoom_Out->setEnabled(!ui->actionFit_Screen->isChecked());
    ui->actionNormal_Size->setEnabled(!ui->actionFit_Screen->isChecked());
}


void MainWindow::zoomAllViewers(ZoomAction action)
{
    qDebug() << "ImageViewerWindow::zoomAllViewers";
    _currentZoomMode = action;
    if(_ignoreZoomEvents || _imageViewers1.count() == 0)
        return;
    _ignoreZoomEvents = true;
    // apply zoom to all viewers
    QMapIterator<QString, ImageViewer3*> it(_imageViewers1);
    int zoomFactor = 0;
    while (it.hasNext())
    {
        it.next();

        if(action == MainWindow::ZOOM_IN)
            it.value()->zoomIn();
        else if(action == MainWindow::ZOOM_OUT)
            it.value()->zoomOut();
        else if(action == MainWindow::ZOOM_FIT)
            it.value()->zoomFit();
        else if(action == MainWindow::ZOOM_RESET)
            it.value()->zoomReset();

        zoomFactor = it.value()->zoomFactor();
    }
    // store the current zoom for new windows
    _currentZoomFactor = static_cast< double >( zoomFactor ) / 100.0;
    on_horizontalScrollBarChanged(((ImageViewer3*)tabWidget->currentWidget())->horizontalScrollBar()->value());
    on_verticalScrollBarChanged(((ImageViewer3*)tabWidget->currentWidget())->verticalScrollBar()->value());
    _ignoreZoomEvents = false;
    on_zoomChanged(zoomFactor);
}

void MainWindow::zoomCurViewers(ZoomAction action){
    qDebug() << "ImageViewerWindow::zoomCurrentViewers";
    _currentZoomMode = action;
    if(_ignoreZoomEvents || _imageViewers1.count() == 0)
        return;
    _ignoreZoomEvents = true;

    ImageViewer3* currImag = (ImageViewer3*)tabWidget->currentWidget();
    // apply zoom to all viewers
    int zoomFactor = 0;
    if(action == MainWindow::ZOOM_IN)
        currImag->zoomIn();
    else if(action == MainWindow::ZOOM_OUT)
        currImag->zoomOut();
    else if(action == MainWindow::ZOOM_FIT)
        currImag->zoomFit();
    else if(action == MainWindow::ZOOM_RESET)
        currImag->zoomReset();
    zoomFactor = currImag->zoomFactor();
    // store the current zoom for new windows
    _currentZoomFactor = static_cast< double >( zoomFactor ) / 100.0;
    on_horizontalScrollBarChanged(((ImageViewer3*)tabWidget->currentWidget())->horizontalScrollBar()->value());
    on_verticalScrollBarChanged(((ImageViewer3*)tabWidget->currentWidget())->verticalScrollBar()->value());
    _ignoreZoomEvents = false;
    on_zoomChanged(zoomFactor);
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    if(event->angleDelta().y() < 0)
        zoomCurViewers(ZOOM_OUT);
    else
        zoomCurViewers(ZOOM_IN);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor){
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::scaleImage(double factor){
//    Q_ASSERT(scrollArea->getImageLabel()->pixmap());
    scaleFactor *= factor;
//    scrollArea->getImageLabel()->resize(scaleFactor * scrollArea->getImageLabel()->pixmap()->size());

////    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
////    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    ui->actionZoom_In->setEnabled(scaleFactor < 3.0);
    ui->actionZoom_Out->setEnabled(scaleFactor > 0.333);
}

void MainWindow::on_actionOpen_O_triggered()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes()){
        mimeTypeFilters.append(mimeTypeName);
    }
    mimeTypeFilters.sort();
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::on_actionPrint_S_triggered()
{
//    Q_ASSERT(imageLabel->pixmap());
//    QPrintDialog dialog(&printer, this);
//    if (dialog.exec()) {
//        QPainter painter(&printer);
//        QRect rect = painter.viewport();
//        QSize size = imageLabel->pixmap()->size();
//        size.scale(rect.size(), Qt::KeepAspectRatio);
//        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//        painter.setWindow(imageLabel->pixmap()->rect());
//        painter.drawPixmap(0, 0, *imageLabel->pixmap());
//    }
}

void MainWindow::on_actionZoom_In_triggered()
{
    zoomCurViewers(ZOOM_IN);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    zoomCurViewers(ZOOM_OUT);
}

void MainWindow::on_actionNormal_Size_triggered()
{
//    scrollArea->getImageLabel()->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::on_actionFit_Screen_triggered()
{
    zoomCurViewers(ZOOM_FIT);
    updateActions();
}



void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About SARATR"),
            tr("<p>The <b>SARATR</b>  shows SAR images auto target recognize."));
}

void MainWindow::tabClose(int index){
    if(_imageViewers1.count()==1){
        return;
    }
    QString index_f = tabWidget->tabText(index);
    delete _imageViewers1.value(index_f);
    _imageViewers1.remove(index_f);
}

void MainWindow::tabChanged(int value){
    //
}

void MainWindow::on_zoomChanged(int val){
    //
}


void MainWindow::on_horizontalScrollBarChanged(int val){

}

void MainWindow::on_verticalScrollBarChanged(int val){

}


void MainWindow::on_mousePositionChanged(int val1, int val2){

}


void MainWindow::on_mouseClick(){

}


void MainWindow::on_mouseDoubleClick(){

}



