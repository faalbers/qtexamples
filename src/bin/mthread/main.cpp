#include <QtGui/QApplication>
#include <iostream>

#include "MThread/MainWindow.h"
#include "MThread/ThreadDialog.h"

using namespace std;
/*
int main(int argv, char **args)
{
    cout << "Running mthread" << endl;
    QApplication app(argv, args);
    MainWindow mainWindow;
    mainWindow.talk();
    mainWindow.show();
    return app.exec();
}
*/
int main(int argv, char **args)
{
    cout << "Running mthread dialog" << endl;
    QApplication app(argv, args);
    ThreadDialog TDialog;
    TDialog.show();
    return app.exec();
}

