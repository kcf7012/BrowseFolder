#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdint.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SetIniPath(char *pFilePath);
    void GetIniPath(char *pData);

    unsigned char *readBitmap(char *name);
    void saveBitmap(char * filename, unsigned char *pImgData, long p_width, long p_height);

    void Cover2DFormat(unsigned char *pImage_in, unsigned char **pImage_out, int nWidth, int nHeight);
    void Cover2DFormat(unsigned char *pImage_in, unsigned char **pImage_out, int nWidth, int nHeight, int bExchangeBits);
    void Cover1DFormat(unsigned char **pImage_in, unsigned char *pImage_out, int nWidth, int nHeight, int bExchangeBits);

    void SaveTXT(char *filename, unsigned char**pData, int nWidth, int nHeight);
    void Save2ByteTXT(char *filename, short**pData, int nWidth, int nHeight);

    unsigned char **p2dDibBits;
    unsigned char **p2dHorizontalVerticalArray;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
