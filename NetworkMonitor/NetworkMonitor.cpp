#include <winsock2.h>

#include "NetworkMonitor.h"
#include "PortScanner.h"

NetworkMonitor::NetworkMonitor(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    {
        PortScanner ps;
    }

}

NetworkMonitor::~NetworkMonitor()
{}


/*
* Closes window when close button is clicked
*/
void NetworkMonitor::on_closeButton_clicked()
{
    close();
}