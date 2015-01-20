#ifndef __THREAD__
#define __THREAD__

#include <QtCore/QThread>
#include <QtCore/QSharedPointer>
#include <QtGui/QImage>

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(int width, int height);

    void stop();

signals:
    void partialRendering(QImage *partial);
    //void renderingReady(QImage *final);

    protected:
    void run();

private:
    QSharedPointer<QImage>  renderImage; // Will get deleted when out of scope
    volatile bool           stopped;
};

#endif
