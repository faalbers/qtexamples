#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QImage>
#include "ImageUpdate/Thread.h"
#include <QtGui/QGraphicsScene>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL, Qt::WindowFlags flags = NULL);

public slots:
    void displayPartialImage(QImage *partial);

private slots:
    void startThread();
    void stopThread();

private:
    Thread          *thread;
    int             width, height;
    QGraphicsScene  *renderScene;
};


#endif
