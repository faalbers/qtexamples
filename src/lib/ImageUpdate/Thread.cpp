#include "Thread.h"
#include <QColor>
#include <iostream>

using namespace std;

Thread::Thread(int width, int height)
    :   renderImage(new QImage(width, height, QImage::Format_ARGB32)),
        stopped(false)
{
}

void Thread::run()
{
    cout << "Running Thread" << endl;
    for ( int y = 0; y < renderImage->height(); y++ ) {
        for ( int x = 0; x < renderImage->width(); x++ ) {
            if ( !stopped ) {
                renderImage->setPixel(x, y, QColor(0, 0, 0, 255).rgba());
            } else {
                break;
            }
        }
        emit partialRendering(renderImage.data());
    }
    
    cout << "Thread Done" << endl;
}

void Thread::stop()
{
    stopped = true;
}

