#include "FPlot/PlotArea.h"
#include <QPainter>

PlotArea::PlotArea(QWidget* parent)
    :   QWidget(parent),
        hRes(1000),
        vRes(1000),
        hCenter(0.5),
        vCenter(0.5),
        penColor(Qt::white),
        backgroundColor(Qt::black),
        pixmapPtr(NULL),
        painterPtr(NULL)
{
    pixmapPtr = new QPixmap(hRes, vRes);
    pixmapPtr->fill(backgroundColor);
    painterPtr = new QPainter(pixmapPtr);
    painterPtr->setPen(penColor);
}

PlotArea::~PlotArea()
{
    if (pixmapPtr)
        delete(pixmapPtr);
}

QSize PlotArea::sizeHint() const
{
    return QSize(hRes, vRes);
}

void PlotArea::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, *pixmapPtr);
}

void PlotArea::drawFunction(
    double (*plotFunction) (const double&),
    const double& xSize,
    const double& ySize)
{
    int x, y, xLast, yLast;
    double hPixelSize   = xSize / hRes;
    double ySizeInv = 1.0 / ySize;
    int    hPixelOffset = hRes * hCenter;
    int    vPixelOffset = vRes * vCenter;
    double xD, yD;

    xLast = yLast = 0;
    for (x = 0; x < hRes; x++) {
        xD = (x - hPixelOffset) * hPixelSize;
        yD = plotFunction(xD);
        y  = (int) (-yD * ySizeInv * vRes) + vPixelOffset;
         if ( x == 0 )
            painterPtr->drawLine(x, y, x, y);
        else
            painterPtr->drawLine(xLast, yLast, x, y);
        xLast = x;
        yLast = y;
    }
    update();
}


