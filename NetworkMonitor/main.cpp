#include "NetworkMonitor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetworkMonitor w;
    w.show();
    return a.exec();
}
