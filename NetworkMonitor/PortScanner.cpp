#include "PortScanner.h"


/* Need to create multiple threads
	Each thread must attempt to connect to a port number
	
*/

// Constructor
PortScanner::PortScanner()
{
    outfile.open("debug.txt");
    outfile << " === Application Started === " << endl;
}

// Destructor
PortScanner::~PortScanner()
{
    outfile.close();
}

int PortScanner::init()
{
    outfile << " WSAStartup ... ";

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        outfile << "Fail: " << iResult << endl;
        return 1;
    }

    outfile << "Success: " << iResult << endl;

    // Fills a block of memory with zeroes
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_family = AF_UNSPEC;        // Unspecified address family can return IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;    // Application requests sock type to be a stream for TCP
    hints.ai_protocol = IPPROTO_TCP;


    outfile << "Getaddrinfo ... ";
    // Request IP address of server from command line
    iResult = getaddrinfo(ip, "255", &hints, &result);      // Pass 255 as port number for base case
    if (iResult != 0) {
        outfile << "Fail: " << iResult << endl;
        WSACleanup();
        return 1;
    }
    outfile << "Success: " << iResult << endl;
}

void PortScanner::scan()
{

}
