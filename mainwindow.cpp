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



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
//    this->setWindowIcon(QIcon(":/res/logo.ico"));
    permanent = new QLabel;
    permanent->setFrameStyle(QFrame::NoFrame);
    permanent->setText("www.saratr.com");
    ui->statusBar->addPermanentWidget(permanent);

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);
    resize(QGuiApplication::primaryScreen()->availableSize());
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());
        imageLabel->setPixmap(QPixmap());
        imageLabel->adjustSize();
        return false;
    }
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;
    ui->actionPrint_S->setEnabled(true);
    ui->actionFit_Screen->setEnabled(true);
    imageLabel->adjustSize();
    updateActions();
    return true;
}

void MainWindow::updateActions(){
    ui->actionZoom_In->setEnabled(!ui->actionFit_Screen->isChecked());
    ui->actionZoom_Out->setEnabled(!ui->actionFit_Screen->isChecked());
    ui->actionNormal_Size->setEnabled(!ui->actionFit_Screen->isChecked());
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor){
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}


void MainWindow::scaleImage(double factor){
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    ui->actionZoom_In->setEnabled(scaleFactor < 3.0);
    ui->actionZoom_Out->setEnabled(scaleFactor > 0.333);
}

void MainWindow::on_actionOpen_O_triggered()
{
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes()){
        qDebug()<<mimeTypeName;
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
    Q_ASSERT(imageLabel->pixmap());
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = imageLabel->pixmap()->size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(imageLabel->pixmap()->rect());
        painter.drawPixmap(0, 0, *imageLabel->pixmap());
    }
}

void MainWindow::on_actionZoom_In_triggered()
{
    scaleImage(1.25);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    scaleImage(0.8);
}

void MainWindow::on_actionNormal_Size_triggered()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::on_actionFit_Screen_triggered()
{
    bool fitToWindow = ui->actionFit_Screen->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        on_actionNormal_Size_triggered();
    }
    updateActions();
}



void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About SARATR"),
            tr("<p>The <b>SARATR</b>  shows SAR images auto target recognize."));
}
