#include "NetworkMonitor.h"

NetworkMonitor::NetworkMonitor(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
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