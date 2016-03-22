#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImageReader>
#include <QLabel>
#include <QScrollArea>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool loadFile(const QString &);

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

    Ui::MainWindow *ui;
    QLabel *imageLabel;
    QLabel *permanent;
    QScrollArea *scrollArea;
    double scaleFactor;
#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif
};

#endif // MAINWINDOW_H
