#include "inputparser.h"
#include <QTimer>
#include "displayarea.h"
#include <QRandomGenerator>
#include <QDebug>

InputParser::InputParser(FILE* input,DisplayArea* display)
{
    InputParser::input=input;

    InputParser::display=display;

    random=new QRandomGenerator();

    /*QTimer* timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(process()));
    timer->start(0);*/
}

void InputParser::sendParameters(unsigned long ignoreFirstNLines,QString palettePath)
{
    this->ignoreFirstNLines=ignoreFirstNLines;
    if(palettePath!=NULL){
        usePalette=true;
        palette=QImage(palettePath);
    }
    else{
        palette=QImage(255,1,QImage::Format_ARGB32);
        QRgb* palettePixels=(QRgb*)palette.bits();
        for(unsigned int i=0;i<palette.sizeInBytes()/sizeof(QRgb);i++) palettePixels[i]=qRgb(i,i,i);
    }
}

void InputParser::process()
{
    while(1){
    int c=fgetc(input);
    if(c==EOF) continue;

    if(nbOfLinesParsed<ignoreFirstNLines){
        if(c=='\n') nbOfLinesParsed++;
        continue;
    }

    switch(c){
        case ',':
            switch(index){
                case 1:
                    computeEpochDate();
                    if(nbOfLinesParsed==ignoreFirstNLines){
                        startDate=currentLine.parsedDate;
                        endDate=currentLine.parsedDate;
                    }
                    startDate=qMin(currentLine.parsedDate,startDate);
                    endDate=qMax(currentLine.parsedDate,endDate);
                break;
                case 3:
                    if(currentLine.maxFreq==maxFreq) currentY++;

                    minFreq=qMin(minFreq,currentLine.minFreq);
                    maxFreq=qMax(maxFreq,currentLine.maxFreq);
                break;
                case 4:
                    if(decimal) currentLine.freqStep++;
                    freqStep=currentLine.freqStep;
                break;
                default:
                    if(index>5){
                        sendPixel();
                    }
                break;
            }
            index++;
            decimal=false;
        break;
        case '\n':
            sendPixel();

            display->updateScale(minFreq,maxFreq,freqStep,10,&startDate,&endDate);

            lastMaxFreq=currentLine.maxFreq;

            index=0;
            nbOfLinesParsed++;
            currentLine=line();
            decimal=false;
        break;
        case '.':
            decimal=true;
        break;
        case '-':
            sign=-1;
            switch(index){
                case 0:
                    addToString(c,&currentLine.firstDate);
                break;
                case 1:
                    addToString(c,&currentLine.secondDate);
                break;
            }
        break;
        default:
            switch(index){
                case 0:
                    addToString(c,&currentLine.firstDate);
                break;
                case 1:
                    addToString(c,&currentLine.secondDate);
                break;
                case 2:
                    addToInt(c,&currentLine.minFreq);
                break;
                case 3:
                    addToInt(c,&currentLine.maxFreq);
                break;
                case 4:
                    addToInt(c,&currentLine.freqStep);
                break;
                default:
                    if(index>5) addToFloat(c,&currentPowerValue);
                break;
            }
        break;
    }
    }
}

void InputParser::addToString(char c, QString* str)
{
    str->append(c);
}

void InputParser::addToInt(char c, unsigned long* nb)
{
    if(decimal||c<'0'||c>'9') return;
    *nb*=10;
    *nb+=(c-'0');
}

void InputParser::addToFloat(char c, float* nb)
{
    if(c<'0'||c>'9') return;
    if(!decimal){
        *nb*=10;
        *nb+=(c-'0');
    }
    else{
        float inc=((c-'0')/pow(10,decimalIndex));
        *nb+=inc;
        decimalIndex++;
    }
}

void InputParser::computeEpochDate()
{
    QString dateMerged=QString();
    dateMerged.append(currentLine.firstDate);
    dateMerged.append(currentLine.secondDate);
    //2021-05-28 23:20:36
    currentLine.parsedDate=QDateTime::fromString(dateMerged,"yyyy-MM-dd hh:mm:ss");
}

void InputParser::sendPixel()
{
    currentPowerValue*=sign;

    if(currentPowerValue<minPow||currentPowerValue>maxPow){
        float newMin=qMin(currentPowerValue,minPow);
        float newMax=qMax(currentPowerValue,maxPow);

        display->updateColorMap(minPow,maxPow,newMin,newMax,palette);

        minPow=newMin;
        maxPow=newMax;
    }

    QRgb color;
    double normCol=(currentPowerValue-minPow)/(maxPow-minPow);
    QRgb* palettePixels=(QRgb*)palette.bits();
    unsigned long paletteIndex=qRound(palette.sizeInBytes()/sizeof(QRgb)*normCol);
    color=palettePixels[paletteIndex];

    currentX=(currentLine.minFreq-minFreq)/currentLine.freqStep+(index-5);

    display->setPixel(currentX,currentY,color);
    currentX++;
    currentPowerValue=0;
    decimalIndex=1;
    sign=1;
}
