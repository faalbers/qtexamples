#include "OGL/MainWindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setWindowTitle(tr("OpenGL Box"));

    glWidget = new GLWidget;
    
    setCentralWidget(glWidget);
}
