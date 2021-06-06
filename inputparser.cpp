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


void InputParser::process()
{
    while(1){
    int c=fgetc(input);
    if(c==EOF) continue;

    switch(c){
        case ',':
            switch(index){
                case 1:
                    computeEpochDate();
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
            index=0;
            minFreq=qMin(minFreq,currentLine.minFreq);
            maxFreq=qMax(maxFreq,currentLine.maxFreq);

            sendPixel();

            if(currentLine.maxFreq<=lastMaxFreq){
                currentY++;
                currentX=0;
            }
            lastMaxFreq=currentLine.maxFreq;

            currentLine=line();
            decimal=false;
        break;
        case '.':
            decimal=true;
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
    //printf("%c aaa");
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

        display->updateColorMap(minPow,maxPow,newMin,newMax);

        minPow=newMin;
        maxPow=newMax;
    }

    int col=qRound(currentPowerValue-minPow)*255/(maxPow-minPow);
    display->setPixel(currentX,currentY,qRgba(col,col,col,255));
    currentX++;
    currentPowerValue=0;
    decimalIndex=1;
    sign=1;
}
