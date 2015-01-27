#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "Thread.h"
#include <QGraphicsScene>

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
