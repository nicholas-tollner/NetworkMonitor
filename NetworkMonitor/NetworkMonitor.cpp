#include <winsock2.h>
#include <fstream>

#include "NetworkMonitor.h"
#include "PortScanner.h"

#define PORTS 100

/**
 * Constructor for NetworkMonitor.
 * Handles GUI setup and clears outfile.
 * \param parent
 */
NetworkMonitor::NetworkMonitor(QWidget *parent): QWidget(parent)
{
    // Setup GUI
    ui.setupUi(this);

    // Ensure output file is clear for storing ping results
    ofstream outfile("output.txt");
    outfile.close();
}

/**
 * Called when user presses close button on GUI.
 * Handles closing of the application.
 */
void NetworkMonitor::on_closeButton_clicked()
{
    close();
}

/**
 * Called when user presses scan button on GUI.
 * Handles port scanning by passing IP & PortNumber to Portscanner instance.
 * Not working as intended, needs to run on separate thread to GUI.
 */
void NetworkMonitor::on_scanButton_clicked()
{
    string ip = "127.0.0.1";
    int portNumber = 54321;
    string message = "Hello!";

    updateProgress(0);                          // Pass 0 meaning scan

    // Setup PortScanner
    for (int i = 0; i < 5; i++)
    {
        std::string result = "Pinging " + ip + ":" + to_string(portNumber + i) + " . . . ";
        std::thread t1([this, &ip, &portNumber, &message, &result, &i] {  
            PortScanner ps(ip);                // Localhost 
            ps.init(portNumber + i);                        // Port number to check
            if (ps.scan(portNumber + i, message) != 1)
            {
                result += "Success!";
                ps.close();
            }
            else {
                result += "Failed";
            }
        });
        t1.join();
        updateProgress(1);                   // Pass 1 meaning scanning complete
        updateMessageList(result);
    }
}

/**
 * Updates GUI label to display progress of background port scanning process.
 * Not working as intended, needs to run on GUI thread.
 * \param flag Used to decide what text to display.
 */
void NetworkMonitor::updateProgress(int flag) 
{
    string current = ui.progressLabel->text().toStdString();

    switch (flag)
    {
        case 0:
            ui.progressLabel->setText("Scanning . . .");
            break;
        case 1:
            ui.progressLabel->setText("Done");
            break;
        default:
            break;
    }
}

/**
 * Updates GUI list with result of background port scanning process.
 * Not working as intended, needs to run on GUI thread.
 * \param message The text to be displayed.
 */
void NetworkMonitor::updateMessageList(std::string message)
{
    ui.messageList->addItem(QString::fromStdString(message));
}