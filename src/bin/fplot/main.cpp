#include <QApplication>
#include "FPlot/MainWindow.h"
#include <math.h>

//using namespace std;

double plotFunctionA(const double& x)
{
    //return x*x + 2*x + 3;
    return x*x;
}

double plotTangentA(const double& x)
{
    //return 2*x + 2;
    double xAt = 1.0;
    double yAt = plotFunctionA(xAt);

    // gradient of x*x = 2x
    double m    = 2*xAt;

    return m*(x-xAt)+yAt;
}

double plotNormalA(const double& x)
{
    //return 2*x + 2;
    double xAt = 1.0;
    double yAt = plotFunctionA(xAt);

    // gradient of x*x = 2x
    double m    = 2*xAt;

    return (-1/m)*(x-xAt)+yAt;
}

int main(int argv, char **args)
{
    QApplication app(argv, args);
    MainWindow mainWindow;

    double xSize = 10.0;
    double ySize = 10.0;
    mainWindow.drawFunction(plotFunctionA, xSize, ySize);
    mainWindow.drawFunction(plotTangentA, xSize, ySize);
    mainWindow.drawFunction(plotNormalA, xSize, ySize);
   
    mainWindow.show();
    return app.exec();
}

