#include "NetworkMonitor.h"
#include <QtWidgets/QApplication>

#pragma comment(lib,"WS2_32")       // Provide external symbols to linker

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    NetworkMonitor widget;

    widget.show();

    return app.exec();              // Enters main event loop for GUI
}
