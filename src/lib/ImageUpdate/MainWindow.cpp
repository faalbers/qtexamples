#include "ImageUpdate/MainWindow.h"
#include <QtGui/QWidget>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QGraphicsView>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    :   QMainWindow(parent, flags),
        thread(NULL),
        renderScene(NULL),
        width(500),
        height(500)
{
    setWindowTitle(tr("Image Updater"));

    QWidget *central = new QWidget;
    QHBoxLayout *hlayout = new QHBoxLayout;
    central->setLayout(hlayout);
    
    QVBoxLayout *buttonsLayout = new QVBoxLayout;
    hlayout->addLayout(buttonsLayout);
    
    QPushButton *runThread = new QPushButton(tr("&Run"));
    QPushButton *stopThread = new QPushButton(tr("&Run"));
    connect(runThread, SIGNAL(clicked()),
        this, SLOT(startThread()));
    connect(stopThread, SIGNAL(clicked()),
        this, SLOT(stopThread()));
        
    buttonsLayout->addWidget(runThread);
    buttonsLayout->addWidget(stopThread);
    buttonsLayout->addStretch();
    
    QGraphicsView *renderView = new QGraphicsView;
    renderScene = new QGraphicsScene;
    renderScene->setSceneRect(0, 0, width, height);
    renderView->setScene(renderScene);
    hlayout->addWidget(renderView);
    
    setCentralWidget(central);
}

void MainWindow::startThread()
{
    stopThread();
    thread = new Thread(width, height);
    connect(thread, SIGNAL(partialRendering(QImage*)),this, SLOT(displayPartialImage(QImage*)));
    thread->start();
}

void MainWindow::stopThread()
{
    if (thread) {
        thread->stop();
        thread->wait();
        delete thread;
        thread = NULL;
    }
}

void MainWindow::displayPartialImage(QImage *partial)
{
    renderScene->clear();
    renderScene->addPixmap(QPixmap::fromImage(*partial));
    cout << "Line render" << endl;
}

