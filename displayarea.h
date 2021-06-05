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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void resizeImage(QImage *image, const QSize &newSize);

    QImage image;

public slots:
    void updateDisp();

signals:

};

#endif // DISPLAYAREA_H
