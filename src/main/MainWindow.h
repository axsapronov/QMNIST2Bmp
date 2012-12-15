#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "ui_MainWindow.h"

class MNISTtoBmpConverter : public QMainWindow
{
    Q_OBJECT

public:
    MNISTtoBmpConverter(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~MNISTtoBmpConverter();

    void Convert(QString imagesFile, QString labelsFile, QString outDir);

private slots:
    void browseImages();
    void browseLabel();
    void browseOutputFolder();
    void convert();

private:
    Ui::MNISTtoBmpConverterClass ui;
};

#endif // MAINWINDOW_H
