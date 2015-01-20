#include "MThread/MainWindow.h"

MainWindow::MainWindow(
    QWidget *parent,
    Qt::WindowFlags flags)
    :   QMainWindow(parent, flags)
{
    cout << "Creating MThread MainWindow" << endl;
    setWindowTitle(tr("Threading test"));
}

MainWindow::~MainWindow()
{
}

