#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTreeView>
#include <QMessageBox>
#include <QThread>
#include <QTime>
#include <QDebug>
#include <QDir>
#include <QStringList>
#include <QList>
#include <QByteArray>
#include <QTextCodec>
#include <QTextEdit>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLEEP_TIME_MILL 10000

#pragma pack( push, 1 ) // avoid structure padding... I am using Win32 here
typedef struct
{
    uint16_t		bfType;
    uint32_t		bfSize;
    uint16_t		bfReserved1;
    uint16_t		bfReserved2;
    uint32_t		bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    uint32_t		biSize;
    int32_t			biWidth;
    int32_t			biHeight;
    uint16_t		biPlanes;
    uint16_t		biBitCount;
    uint32_t		biCompression;
    uint32_t		biSizeImage;
    int32_t			biXPelsPerMeter;
    int32_t			biYPelsPerMeter;
    uint32_t		biClrUsed;
    uint32_t		biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop) // avoid structure padding... I am using Win32 here

#define IMAGE_WIDTH     80
#define IMAGE_HEIGHT    80
int IMG_WIDTH = IMAGE_WIDTH;
int IMG_HEIGHT = IMAGE_HEIGHT;
char strGlobalPath[256] = { 0 };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    char buff[80] = { 0 };
    GetIniPath(buff);
    qDebug("#line%d buff = %s\r\n", __LINE__, buff);

    char *str = NULL;
    str = buff;
    QString qstr = QString(QLatin1String(str));
    ui->textEdit->setText(qstr);
    QFont textFont("Helvetica");
    textFont.setStyleHint(QFont::SansSerif);
    ui->textEdit->setFont(textFont);

    ui->progressBar->setValue(0);

    p2dDibBits = NULL;
    p2dHorizontalVerticalArray = NULL;

    p2dDibBits = (unsigned char **)malloc(sizeof(unsigned char *) * IMG_HEIGHT);
    p2dHorizontalVerticalArray = (unsigned char **)malloc(sizeof(unsigned char *) * IMG_HEIGHT);
    for (int j = 0; j < IMG_HEIGHT; j++) {
        p2dDibBits[j] = (unsigned char *)malloc(sizeof(unsigned char) * IMG_WIDTH);
        memset(p2dDibBits[j], 255, IMG_WIDTH);
        p2dHorizontalVerticalArray[j] = (unsigned char *)malloc(sizeof(unsigned char) * IMG_WIDTH);
        memset(p2dHorizontalVerticalArray[j], 255, IMG_WIDTH);
    }
}

MainWindow::~MainWindow()
{
    if (p2dHorizontalVerticalArray != NULL) {
        for (int j = 0; j < IMG_HEIGHT; j++) {
            free(p2dHorizontalVerticalArray[j]);
        }
        free(p2dHorizontalVerticalArray);
        p2dHorizontalVerticalArray = NULL;
    }

    if (p2dDibBits != NULL) {
        for (int j = 0; j < IMG_HEIGHT; j++) {
            free(p2dDibBits[j]);
        }
        free(p2dDibBits);
        p2dDibBits = NULL;
    }

    delete ui;
}

void MainWindow::SetIniPath(char *pFilePath)
{
    if(pFilePath == NULL)
        return;

    FILE *fp_write = NULL;
    //char *filename = "Setup.ini";
    char filename[] = "Setup.ini";
    char pStrPath[256] = { 0 };

    sprintf(pStrPath, "%s", filename);
    qDebug("#line%d pStrPath = %s", __LINE__, pStrPath);

    fp_write = fopen(pStrPath, "w");
    if (!fp_write) {
        qDebug("Open File Failed");
        return;
    }
    fwrite(pFilePath, 1, strlen(pFilePath), fp_write);
    fclose(fp_write);
}

void MainWindow::GetIniPath(char *pData)
{
    FILE *fp_read = NULL;
    //char *filename = "Setup.ini";
    char IniName[] = "Setup.ini";
    char *filename = IniName;

    char pStrPath[256] = { 0 };
    char Buff_read[80] = { 0 };

    sprintf(pStrPath, "%s", filename);
    qDebug("#line%d pStrPath = %s\r\n", __LINE__, pStrPath);

    fp_read = fopen(pStrPath, "r");
    if (!fp_read) {
        qDebug("Open File Failed");
        return;
    }

    //Error Type -> C:\Program Files
    //while (fscanf(fp_read, "%s", Buff_read) != EOF) {
    //	qDebug("#line%d %s\r\n", __LINE__, Buff_read);
    //}

    if(fgets(Buff_read , 80 , fp_read) != NULL ) {
        qDebug("#line%d %s", __LINE__, Buff_read);
    }

    qDebug("#line%d strlen(Buff_read) = %d", __LINE__, strlen(Buff_read));//18 Bytes
    qDebug("#line%d sizeof(Buff_read) = %d", __LINE__, sizeof(Buff_read));//80 Bytes

    memcpy(pData, Buff_read, strlen(Buff_read));
    pData[strlen(Buff_read)+1] = '\0';
    qDebug("#line%d strlen(Buff_read) = %d", __LINE__, strlen(Buff_read));//18 Bytes
    fclose(fp_read);
}

void MainWindow::on_pushButton_clicked()
{
    QTime startTime = QTime::currentTime();
    QThread::msleep(SLEEP_TIME_MILL);

    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), tr("C://"), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(path.isEmpty()){
        qDebug("isEmpty");
        return;
    }

    QMessageBox::information(this, tr("Directory"), path);
    QDir *dir = new QDir(path);
    QStringList filter;
    filter << "*.bmp";
    dir->setNameFilters(filter);

    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
    qDebug() << "BMP File is :" << fileInfo->count();
    ui->progressBar->setRange(0, fileInfo->count());
    for(int i = 0; i < fileInfo->count(); i++) {
         qDebug() << fileInfo->at(i).fileName();
         //qDebug() << fileInfo->at(i).filePath();
         ui->progressBar->setValue(i + 1);
         QCoreApplication::processEvents();
    }

    //QFileInfoList fileInfoList = dir->entryInfoList(filter);
    //qDebug() << "BMP File is :" << fileInfoList.count();
    //for (int i = 0; i < fileInfoList.count(); i++) {
    //    qDebug() << fileInfoList[i].fileName();
    //}

    QByteArray ba = path.toLocal8Bit();
    char *c_str2 = ba.data();
    qDebug("str2: %s", c_str2);
    SetIniPath(c_str2);

    QTime stopTime = QTime::currentTime();
    int elapsed = startTime.msecsTo(stopTime);

    qDebug() << "Directory is :" << path;
    qDebug() << "QTime elapsed :" << elapsed << "ms";
    //qDebug("QTime elapsed : %d ms", elapsed);

    ui->textEdit->setText(path);
    QFont textFont("Helvetica");
    textFont.setStyleHint(QFont::SansSerif);
    ui->textEdit->setFont(textFont);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString path;
    path = ui->textEdit->toPlainText();
    qDebug() << "#line" << __LINE__ << path;
    if(path.isEmpty()){
        qDebug("isEmpty");
        return;
    }

    QMessageBox::information(this, tr("Directory"), path);
    QDir *dir = new QDir(path);
    QStringList filter;
    filter << "*.bmp";
    dir->setNameFilters(filter);

    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
    qDebug() << "BMP File is :" << fileInfo->count();
    ui->progressBar->setRange(0, fileInfo->count());
    for(int i = 0; i < fileInfo->count(); i++) {
         //qDebug() << fileInfo->at(i).fileName();
         qDebug() << fileInfo->at(i).filePath();

         //QByteArray ba = fileInfo->at(i).filePath().toLocal8Bit();
         //char *filename = ba.data();
         //qDebug("filename: %s", filename);

         ui->progressBar->setValue(i + 1);
         QCoreApplication::processEvents();
         QThread::sleep(1);
    }
}

unsigned char *MainWindow::readBitmap(char *filename)
{
    if (filename == NULL) {
        qDebug("Error bmpRead.");
        return NULL;
    }
    qDebug() << "readBitmap name:" << filename;

    FILE *filePtr; //our file pointer
    BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
    BITMAPINFOHEADER bitmapInfoHeader;
    unsigned char *bitmapImage;  //store image data
    //int imageIdx=0;  //image index counter
    //unsigned char tempRGB;  //our swap variable

    //open filename in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL)
        return NULL;

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

    //verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.bfType !=0x4D42) {
        fclose(filePtr);
        return NULL;
    }

    //read the bitmap info header
    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr); // small edit. forgot to add the closing bracket at sizeof

    //move file point to the begging of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

    //allocate enough memory for the bitmap image data
    bitmapImage = (unsigned char*)malloc(bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight);

    //verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    //read in the bitmap image data
    fread(bitmapImage, sizeof(unsigned char), bitmapInfoHeader.biWidth*bitmapInfoHeader.biHeight ,filePtr);

    //make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        return NULL;
    }

    //close file and return bitmap iamge data
    fclose(filePtr);
    return bitmapImage;
}

void MainWindow::saveBitmap(char * filename, unsigned char *pImgData, long p_width, long p_height)
{
    long i, y;
    BITMAPFILEHEADER bitmapFileHeader;
    memset(&bitmapFileHeader, 0, sizeof(BITMAPFILEHEADER));
    bitmapFileHeader.bfType = ('B' | 'M' << 8);
    bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (4*256);
    bitmapFileHeader.bfSize = bitmapFileHeader.bfOffBits + (p_width + (p_width % 4 ? (4 - p_width % 4) : 0)) * p_height; // multiply by 3 if you wanna switch to RGB

    BITMAPINFOHEADER bitmapInfoHeader;
    memset(&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
    bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfoHeader.biWidth = p_width;
    bitmapInfoHeader.biHeight = p_height;
    bitmapInfoHeader.biPlanes = 1;
    bitmapInfoHeader.biBitCount =  8; // this means grayscale, change to 24 if you wanna switch to RGB

    FILE * ff;
    ff = fopen(filename, "wb");
    if (ff == NULL)
      return ;

    fwrite(&bitmapFileHeader, sizeof(bitmapFileHeader), 1, ff);
    fwrite(&bitmapInfoHeader, sizeof(bitmapInfoHeader), 1, ff);

    // bitmaps grayscale must have a table of colors... don't write this table if you want RGB
    unsigned char grayscale[4];
    memset(grayscale, 0, sizeof(grayscale));
    for (i = 0; i < 256; ++i)
    {
        memset(grayscale, i, sizeof(grayscale)-1);
        fwrite(grayscale, sizeof(grayscale), 1, ff);
    }

    // here we record the pixels... I created a gradient...
    // remember that the pixels ordem is from left to right, "bottom to top"...
    int nRawBytes = p_width + (p_width % 4 ? (4 - p_width % 4) : 0);
    unsigned char *Pixels = (unsigned char*) malloc(nRawBytes);
    memset(Pixels, 0, nRawBytes);

    for (y = 0; y < p_height; y++)
    {
        memcpy(Pixels, &pImgData[p_width*y], p_width);
        fwrite(Pixels, 1, nRawBytes, ff);
    }

    free(Pixels);
    fclose(ff);
}

void MainWindow::Cover2DFormat(unsigned char *pImage_in, unsigned char **pImage_out, int nWidth, int nHeight)
{
    if(pImage_in == NULL || pImage_out == NULL)
        return;

    int i, j;
    for (j = 0; j < nHeight; j++) {
        for (i = 0; i < nWidth; i++) {
            pImage_out[j][i] = pImage_in[j * nWidth + i];
        }
    }
}

void MainWindow::Cover2DFormat(unsigned char *pImage_in, unsigned char **pImage_out, int nWidth, int nHeight, int bExchangeBits)
{
    if(pImage_in == NULL || pImage_out == NULL)
        return;

    int i, j;
    for (j = 0; j < nHeight; j++) {
        for (i = 0; i < nWidth; i++) {
            if(bExchangeBits == 0)
                pImage_out[j][i] = pImage_in[j * nWidth + i];
            else
                pImage_out[nHeight - 1 - j][i] = pImage_in[j * nWidth + i];
        }
    }

}

void MainWindow::Cover1DFormat(unsigned char **pImage_in, unsigned char *pImage_out, int nWidth, int nHeight, int bExchangeBits)
{
    if (pImage_in == NULL || pImage_out == NULL)
        return;

    int i, j;
    for (j = 0; j < nHeight; j++) {
        for (i = 0; i < nWidth; i++) {
            if(bExchangeBits == 0)
                pImage_out[j * nWidth + i] = pImage_in[j][i];
            else
                pImage_out[(nHeight - 1 - j) * nWidth + i] = pImage_in[j][i];
        }
    }
}

void MainWindow::SaveTXT(char *filename, unsigned char**pData, int nWidth, int nHeight)
{
    if (filename == NULL || pData == NULL || nWidth == 0 || nHeight == 0)
        return;

    int i, j;
    char str[128] = { 0 };
    char pStrPath[256] = { 0 };
    sprintf(pStrPath, "%s", filename);
    FILE *file = fopen(pStrPath, "w");
    if (!file) {
        return;
    }

    for (j = 0; j < nHeight; j++) {
        for (i = 0; i < nWidth; i++) {
            if (i == nWidth - 1) {
                sprintf(str, "%4d,\n", pData[j][i]);
            }
            else {
                sprintf(str, "%4d,", pData[j][i]);
            }
            fwrite(str, 1, strlen(str), file);
        }
    }
    fclose(file);
}

void MainWindow::Save2ByteTXT(char *filename, short**pData, int nWidth, int nHeight)
{
    if (filename == NULL || pData == NULL || nWidth == 0 || nHeight == 0)
        return;

    int i, j;
    char str[128] = { 0 };
    char pStrPath[256] = { 0 };
    sprintf(pStrPath, "%s", filename);

    FILE *file = fopen(pStrPath, "w");
    if (!file) {
        return;
    }

    for (j = 0; j < nHeight; j++) {
        for (i = 0; i < nWidth; i++) {
            if (i == nWidth - 1) {
                sprintf(str, "%4d,\n", pData[j][i]);
            }
            else {
                sprintf(str, "%4d,", pData[j][i]);
            }
            fwrite(str, 1, strlen(str), file);
        }
    }
    fclose(file);
}
