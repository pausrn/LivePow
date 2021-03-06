#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include <QWidget>

class DisplayArea : public QWidget
{
    Q_OBJECT
public:
    DisplayArea(QWidget *parent = nullptr);
    void setPixel(int x,int y,unsigned int col);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void updateColorMap(float oldMin,float oldMax,float newMin,float newMax,QImage palette);
    bool pixelSet(int x,int y);
    void updateScale(unsigned long minFreq, unsigned long maxFreq, unsigned long freqStep, unsigned long timeStep,QDateTime* startDate,QDateTime* endDate);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);

    unsigned long remap(unsigned long val,float omin,float omax,float nmin,float nmax);
    unsigned long findPaletteIndex(QRgb col,QRgb* palette,unsigned long size);

    void displayScale(QPainter* painter);

    unsigned long minFreq=0;
    unsigned long maxFreq=0;
    unsigned long freqStep=0;
    unsigned long timeStep=0;
    QDateTime* startDate;
    QDateTime* endDate;

    QImage image;

public slots:
    void updateDisp();

signals:

};

#endif // DISPLAYAREA_H
