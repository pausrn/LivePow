#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCommandLineParser argsParser;
    argsParser.setApplicationDescription("A live heatmap creator for soapy_power");
    argsParser.addHelpOption();
    argsParser.addPositionalArgument("source","csv file to read data from, default stdin");

    QCommandLineOption skipFirstLines(QStringList() << "s" << "skipFirstLines",
              "Skip the first <n> lines",
              "number of lines");
    argsParser.addOption(skipFirstLines);

    QCommandLineOption palette(QStringList() << "p" << "palette",
              "Use the palette specified in the file",
              "Path to the file");
    argsParser.addOption(palette);

    argsParser.process(a);

    MainWindow w;
    w.sendArgs(&argsParser);
    w.show();
    return a.exec();
}
