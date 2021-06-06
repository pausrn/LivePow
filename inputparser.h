#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <stdio.h>
#include <QTimerEvent>
#include <QObject>
#include <QRandomGenerator>
#include <QDateTime>

class DisplayArea;

struct line{
    QString firstDate;
    QString secondDate;
    QDateTime parsedDate;
    unsigned long minFreq=0;
    unsigned long maxFreq=0;
    unsigned long freqStep=0;
};

class InputParser : public QObject
{
    Q_OBJECT
public:
    InputParser(FILE* input,DisplayArea* display);

    void sendParameters(unsigned long ignoreFirstNLines);

private:
    unsigned long minFreq=-1;
    unsigned long maxFreq=0;

    float minPow=__FLT_MAX__;
    float maxPow=0;

    unsigned long lastMaxFreq=0;

    unsigned long index=0;

    unsigned long nbOfLinesParsed=0;
    unsigned long ignoreFirstNLines=0;

    line currentLine;

    FILE* input;

    bool decimal=false;
    int decimalIndex=1;
    int sign=1;

    unsigned int currentX=0;
    unsigned int currentY=0;

    DisplayArea* display;

    QRandomGenerator* random;

    float currentPowerValue=0;

    void addToString(char c,QString* str);
    void addToInt(char c,unsigned long* nb);
    void addToFloat(char c,float* nb);
    void computeEpochDate();

    void sendPixel();

public slots:
    void process();
};

#endif // INPUTPARSER_H
