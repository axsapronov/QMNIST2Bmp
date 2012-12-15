#include "MainWindow.h"

// for convert
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QImage>

// for gui
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QDesktopWidget> /// moved to center

MNISTtoBmpConverter::MNISTtoBmpConverter(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);


    //    Convert("TestImages.dat", "TestLabels.dat", "testdata");
    //    Convert("TestImages.dat", "TestLabels.dat", "testdata");

    ui.LEImages->setText("TestImages.dat");
    ui.LELabels->setText("TestLabels.dat");
    ui.LEOutputFolder->setText(QDir::currentPath() + "/testdata");

    connect(ui.pBConvert, SIGNAL(clicked()), SLOT(convert()));
    connect(ui.pBImages, SIGNAL(clicked()), SLOT(browseImages()));
    connect(ui.pBLabes,SIGNAL(clicked()), SLOT(browseLabel()));
    connect(ui.pBOutputFolder, SIGNAL(clicked()), SLOT(browseOutputFolder()));
    connect(ui.actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));


    /// moved to center desktop
    QRect rect = QApplication::desktop()->availableGeometry(this);
    this->move(rect.width() / 2 - this->width() / 2,
               rect.height() / 2 - this->height() / 2);
    /// maximized
    //    this->showMaximized();
}
//------------------------------------------------------------------------------
MNISTtoBmpConverter::~MNISTtoBmpConverter()
{

}
//------------------------------------------------------------------------------
void MNISTtoBmpConverter::Convert(QString imagesFileName, QString labelsFileName, QString outDirName)
{
    typedef unsigned char byte;

    QFile imagesFile(imagesFileName);
    imagesFile.open(QIODevice::ReadOnly);
    qDebug() << imagesFile.fileName();
    QFile labelsFile(labelsFileName);
    labelsFile.open(QIODevice::ReadOnly);
    QDataStream imagesIn(&imagesFile);
    QDataStream labelsIn(&labelsFile);

    int nL;
    labelsIn >> nL; //skipping magic number
    labelsIn >> nL;
    int nI;
    imagesIn >> nI;
    imagesIn >> nI; //skipping magic number
    int w, h;
    imagesIn >> w;
    imagesIn >> h;

    QDir outDir;
    outDir.mkpath(outDirName);
    outDir.cd(outDirName);

    byte l;
    byte px;

    int numbersN[10] = { 0 };

    for(int n = 0; n < nL; n++)
    {
        labelsIn >> l;

        QImage image(w, h, QImage::Format_RGB32);
        QString imageName = QString::number(l) + "_" + QString("%1").arg(numbersN[l], 5, 10, QChar('0')) + ".bmp";

        for(int i = 0; i < h; i++)
            for(int j = 0; j < w; j++)
            {
                imagesIn >> px;
                byte c = 255 - px;
                image.setPixel(j, i, c + c * 256 + c * 256 * 256);
            }

        //QIcon icon;
        //icon.addPixmap(pixmap);
        //QImage image = icon.pixmap(pixmap.size(), QIcon::Disabled).toImage();
        //image = image.convertToFormat(QImage::Format_Indexed8);
        //image.save(outDir.absolutePath() + "/" + imageName, "BMP");
        image.save(outDir.absolutePath() + "/" + imageName, "BMP");

        numbersN[l]++;
    }

    QMessageBox::information(0, "Generate", "Operation Complete");
    labelsFile.close();
    imagesFile.close();
}
//------------------------------------------------------------------------------
void MNISTtoBmpConverter::browseOutputFolder()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    QString directory = QFileDialog::getExistingDirectory(this,
                                                          tr("Select output folder"),
                                                          "",
                                                          options);
    if (!directory.isEmpty())
        ui.LEOutputFolder->setText(directory);
}
//------------------------------------------------------------------------------
void MNISTtoBmpConverter::browseLabel()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select label file"),
                                                    "",
                                                    tr("All Files (*);;Dat file (*.dat)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty())
        ui.LELabels->setText(fileName);
}
//------------------------------------------------------------------------------
void MNISTtoBmpConverter::browseImages()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select label file"),
                                                    "",
                                                    tr("All Files (*);;Dat file (*.dat)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty())
        ui.LEImages->setText(fileName);
}
//------------------------------------------------------------------------------
void MNISTtoBmpConverter::convert()
{
    Convert(ui.LEImages->text(), ui.LELabels->text(), ui.LEOutputFolder->text());
}
//------------------------------------------------------------------------------


