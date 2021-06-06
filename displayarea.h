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
    void updateColorMap(float oldMin,float oldMax,float newMin,float newMax);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);

    char remap(char val,float omin,float omax,float nmin,float nmax);

    QImage image;

public slots:
    void updateDisp();

signals:

};

#endif // DISPLAYAREA_H
