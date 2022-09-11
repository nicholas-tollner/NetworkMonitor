#include <winsock2.h>
#include <fstream>

#include "NetworkMonitor.h"
#include "PortScanner.h"

#define PORTS 10

// Constructor
NetworkMonitor::NetworkMonitor(QWidget *parent): QWidget(parent)
{
    int portNumber = 54319;
    string message = "Ping";

    // Ensure output file is clear for storing ping results
    ofstream outfile("output.txt");
    outfile.close();

    // Setup GUI
    ui.setupUi(this);

    // Setup PortScanner
    for (int i = 0; i < PORTS; i++)
    {
        std::thread t1([this, portNumber, message, i] {
            PortScanner ps("127.0.0.1");                // Localhost 
            ps.init(portNumber);                        // Port number to check
            ps.scan(portNumber + i, message);
            });

        t1.join();
   }
    
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