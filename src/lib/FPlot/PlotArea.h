#ifndef __PLOTAREA__
#define __PLOTAREA__

#include <QWidget>
#include <QPixmap>

class PlotArea : public QWidget
{
    Q_OBJECT

public:
    PlotArea(QWidget* parent = 0);
    ~PlotArea();

    QSize sizeHint() const;

    void drawFunction(
        double (*plotFunction) (const double&),
        const double& xSize,
        const double& ySize);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int         hRes, vRes;
    double      hCenter, vCenter;
    QColor      penColor, backgroundColor;
    QPixmap*    pixmapPtr;
    QPainter*   painterPtr;
};

#endif
