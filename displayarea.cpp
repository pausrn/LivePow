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
