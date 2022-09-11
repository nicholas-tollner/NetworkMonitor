#include <winsock2.h>
#include <fstream>

#include "NetworkMonitor.h"
#include "PortScanner.h"


// Constructor
NetworkMonitor::NetworkMonitor(QWidget *parent): QWidget(parent)
{
    // Setup PortScanner
    PortScanner ps;
    ps.init();
    
    // Setup GUI
    ui.setupUi(this);

}

// Destructor
NetworkMonitor::~NetworkMonitor()
{

}


// UI Management Functions

/*
* Closes window when close button is clicked
*/
void NetworkMonitor::on_closeButton_clicked()
{
    close();
}