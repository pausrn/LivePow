#include "displayarea.h"
#include <qpainter.h>
#include <QPaintEvent>
#include <QTimer>

DisplayArea::DisplayArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateDisp()));
    timer->start(100);
}

void DisplayArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect=event->rect();
    painter.drawImage(rect,image,rect);
}

void DisplayArea::setPixel(int x, int y, unsigned int col)
{
    if(x>=image.width()||y>=image.height()) resizeImage(&image,QSize(qMax(x+255,image.width()),qMax(y+255,image.height())));
    image.setPixel(x,y,col);
}

void DisplayArea::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size()==newSize) return;

    QImage newImage(newSize,QImage::Format_ARGB32);
    newImage.fill(qRgba(255,255,255,0));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0,0),*image);
    *image=newImage;
}

void DisplayArea::updateDisp()
{
    update();
}

bool DisplayArea::saveImage(const QString &fileName, const char *fileFormat)
{
    return image.save(fileName, fileFormat);
}

void DisplayArea::updateColorMap(float oldMin, float oldMax, float newMin, float newMax)
{
    QRgb* imagePixels=(QRgb*)image.bits();
    for(unsigned long long i=0;i<image.sizeInBytes()/sizeof(QRgb);i++){
        QRgb currentColor=imagePixels[i];
        QRgb newColor=qRgba(remap(qRed(currentColor),oldMin,oldMax,newMin,newMax),remap(qGreen(currentColor),oldMin,oldMax,newMin,newMax),remap(qBlue(currentColor),oldMin,oldMax,newMin,newMax),qAlpha(currentColor));
        imagePixels[i]=newColor;
    }
}

char DisplayArea::remap(char c, float omin, float omax, float nmin, float nmax)
{
    float nVal=(255*(omin-nmin)+c*(omax-omin))/(nmax-nmin);
    return qRound(nVal);
}
