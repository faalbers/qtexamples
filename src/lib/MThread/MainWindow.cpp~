#include "FPlot/MainWindow.h"

MainWindow::MainWindow(
    QWidget *parent,
    Qt::WindowFlags flags)
    :   QMainWindow(parent, flags), plotArea(NULL)
{
    setWindowTitle(tr("Function Plotter"));

    plotArea = new PlotArea;
    
    setCentralWidget(plotArea);
}

MainWindow::~MainWindow()
{
    delete(plotArea);
}

void MainWindow::drawFunction(
    double (*plotFunction) (const double&),
    const double& xSize,
    const double& ySize)
{
    plotArea->drawFunction(plotFunction, xSize, ySize);
}

