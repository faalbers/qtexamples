#ifndef __MT_MAINWINDOW__
#define __MT_MAINWINDOW__

#include <QMainWindow>
//#include <QWidget>

#include <iostream>

using namespace std;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = NULL, Qt::WindowFlags flags = NULL);
    ~MainWindow();

    void talk() { cout << "I am MThread MainWindow" << endl; }
    
private:
};


#endif
