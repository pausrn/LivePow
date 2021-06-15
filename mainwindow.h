#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCommandLineParser>
#include <QScrollArea>

class DisplayArea;
class InputParser;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void sendArgs(QCommandLineParser* argsParser);

    InputParser *parser;

private:
    QScrollArea *scrollArea;
    DisplayArea *displayArea;
    void createActions();
    void createMenus();

    QMenu *saveAsMenu;
    QMenu *fileMenu;

    QList<QAction *> saveAsActs;

    bool saveFile(const QByteArray &fileFormat);

private slots:
    void save();

signals:
    void operate(const QString &);
};
#endif // MAINWINDOW_H
