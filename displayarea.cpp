#include "displayarea.h"
#include <qpainter.h>
#include <QPaintEvent>
#include <QTimer>
#include <QDateTime>

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
    displayScale(&painter);
}

bool DisplayArea::pixelSet(int x, int y)
{
    if(x>=image.width()||y>=image.height()) return false;
    return qAlpha(image.pixel(x,y))==255;
}

void DisplayArea::setPixel(int x, int y, unsigned int col)
{
    if(x>=image.width()||y>=image.height()) resizeImage(&image,QSize(qMax(x+255,image.width()),qMax(y+255,image.height())));
    image.setPixel(x,y,col);
}

void DisplayArea::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size()==newSize) return;

    resize(newSize);

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
    QImage output=QImage(image);
    QPainter painter(&output);
    displayScale(&painter);
    return output.save(fileName, fileFormat);
}

void DisplayArea::updateColorMap(float oldMin, float oldMax, float newMin, float newMax,QImage palette)
{
    QRgb* imagePixels=(QRgb*)image.bits();
    QRgb* palettePixels=(QRgb*)palette.bits();
    unsigned long size=palette.sizeInBytes()/sizeof(QRgb);
    for(unsigned long long i=0;i<image.sizeInBytes()/sizeof(QRgb);i++){
        QRgb currentColor=imagePixels[i];
        if(qAlpha(currentColor)==0) continue;
        unsigned long oldIndex=findPaletteIndex(currentColor,palettePixels,size);
        unsigned long newIndex=remap(oldIndex,oldMin,oldMax,newMin,newMax);
        QRgb newColor=palettePixels[newIndex];
        imagePixels[i]=newColor;
    }
}

unsigned long DisplayArea::findPaletteIndex(QRgb col, QRgb* palette,unsigned long size)
{
    for(unsigned long i=0;i<size;i++) if(col==palette[i]) return i;
    return 0;
}

unsigned long DisplayArea::remap(unsigned long c, float omin, float omax, float nmin, float nmax)
{
    float nVal=(255*(omin-nmin)+c*(omax-omin))/(nmax-nmin);
    return qRound(nVal);
}

void DisplayArea::displayScale(QPainter* painter)
{
    if(!freqStep) return;

    painter->setPen(qRgb(0,0,0));

    int nbOfDiv=5;
    unsigned long freqDiff=maxFreq-minFreq;
    unsigned long binNb=freqDiff/freqStep;
    unsigned long binSteps=binNb/nbOfDiv;
    for(int i=0;i<nbOfDiv;i++){
        unsigned long freq=(binSteps*i*freqStep)+minFreq;
        unsigned long x=binSteps*i;
        painter->drawLine(x,0,x,10);
        painter->drawText(QPoint(x+3,10),QString("%1 Hz").arg(freq));
    }
    for(int i=100;i<image.height();i+=100){
        unsigned long time=(endDate->toMSecsSinceEpoch()-startDate->toMSecsSinceEpoch())*i/image.height()+startDate->toMSecsSinceEpoch();
        QDateTime date=QDateTime::fromMSecsSinceEpoch(time);
        painter->drawLine(0,i,50,i);
        painter->drawText(QPoint(3,i+15),QString(date.toString()));
    }
}

void DisplayArea::updateScale(unsigned long minFreq, unsigned long maxFreq, unsigned long freqStep, unsigned long timeStep,QDateTime* startDate,QDateTime* endDate)
{
    this->minFreq=minFreq;
    this->maxFreq=maxFreq;
    this->freqStep=freqStep;
    this->timeStep=timeStep;
    this->startDate=startDate;
    this->endDate=endDate;
}
