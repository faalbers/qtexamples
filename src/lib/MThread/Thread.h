#ifndef __THREAD__
#define __THREAD__

#include <QThread>

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread();

    void setMessage(const QString &message) { messageStr = message; }
    void stop();

protected:
    void run();

private:
    QString         messageStr;
    volatile bool   stopped;
};

#endif
