#include <stdio.h>
#include "mainwindow.h"
#include "displayarea.h"
#include "inputparser.h"
#include <QTimer>
#include <QThread>
#include <QAction>
#include <QImageWriter>
#include <QMenu>
#include <QDir>
#include <QFileDialog>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),displayArea(new DisplayArea(this))
{
    setCentralWidget(displayArea);

    FILE* input=stdin;

    parser=new InputParser(input,displayArea);

    QThread* workerThread=new QThread();

    parser->moveToThread(workerThread);
    connect(workerThread, &QThread::started, parser, &InputParser::process);
    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    workerThread->start();

    createActions();
    createMenus();

    setWindowTitle(tr("LivePow"));
}

MainWindow::~MainWindow()
{
}

void MainWindow::save(){
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                    .arg(QString::fromLatin1(fileFormat.toUpper()),QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    return displayArea->saveImage(fileName, fileFormat.constData());
}

void MainWindow::createActions()
{
    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
    for (const QByteArray &format : imageFormats) {
        QString text = tr("%1...").arg(QString::fromLatin1(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, &QAction::triggered, this, &MainWindow::save);
        saveAsActs.append(action);
    }
}

void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    for (QAction *action : qAsConst(saveAsActs))
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addMenu(saveAsMenu);

    menuBar()->addMenu(fileMenu);
}
