#include "PortScanner.h"

#define SKIP 3

// Constructor
PortScanner::PortScanner(string ip)
{
    this->ip = ip;

    outfile.open("debug.txt");

    outfile << " === Application Started === " << endl;
}

/**
 * Destructor for PortScanner class.
 * 
 * Handles movement of debug.txt info to output.txt for subsequent scans.
 */
PortScanner::~PortScanner()
{
    outfile.close();
    outfile.open("output.txt", ios_base::app);          // Append mode
    infile.open("debug.txt");


    for (int i = 0; i < SKIP; i++)
    {
        infile.ignore(INT_MAX, '\n');            // Read in and discard characters until delimiter is reached
    }

    while (std::getline(infile, line))
    {
        outfile << line << "\n";
    }

    outfile.close();
    infile.close();
}

/**
 * Handles set up of address info struct.
 * 
 * \param portNumber The port number to connect with.
 * \return int, 1 if error occurred, 0 otherwise
 */
int PortScanner::init(int portNumber)
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
    iResult = getaddrinfo(ip.c_str(), std::to_string(portNumber).c_str(), &hints, &result);
    if (iResult != 0) {
        outfile << "Fail: " << iResult << endl;
        WSACleanup();
        return 1;
    }
    outfile << "Success: " << iResult << endl;
    return 0;
}

/**
 * Sets up socket, handles sending & receiving of data using multithreading.
 * Socket timeout for receiving is set to 100ms.
 * \param portNumber
 * \param message
 * \return 
 */
int PortScanner::scan(int portNumber, string message)
{
    // Point to result addrinfo struct
    ptr = result;

    // Setup socket to use for connection to server
    connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    // Set connectSocket to timeout after 100 milliseconds
    DWORD timeout = 100;           // Wait for data
    setsockopt(connectSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    outfile << "Pinging " << ip << ":" << portNumber << " ... ";

    // Check connectSocket is now setup correctly
    if (connectSocket == INVALID_SOCKET) {
        outfile << "Error: " << WSAGetLastError() << endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Attempt connection
    iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

    // Check if connection was successful
    if (iResult == SOCKET_ERROR) {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    // Check if connection failed
    if (connectSocket == INVALID_SOCKET) {
        outfile << "Error: Unable to connect!" << endl;
        WSACleanup();
        return 1;
    }
    outfile << "Success!" << endl;

    thread send_thread([this, portNumber, message] { sendData(message); });
    thread recv_thread([this] { recvData(); });

    send_thread.join();
    recv_thread.join();

    return 0;
}

/**
 * Handles sending of data on predefined socket connection.
 * 
 * \param message The data to be sent on the socket connection.
 * \return int, 1 if error occurred, 0 otherwise
 */
int PortScanner::sendData(string message)
{
    // Sending data
    // Truncate message length to DEFAULT_BUFLEN
    if (message.length() > DEFAULT_BUFLEN) {
        message = message.substr(0, DEFAULT_BUFLEN);
    }
    sendbuf = message.data();

    // Send input to server
    iResult = send(connectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}

/**
 * Handles receiving of data on predefined socket connection.
 */
void PortScanner::recvData()
{
    do {
        // Receive data then return
        iResult = recv(connectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            for (int i = 0; i < iResult; i++) {
                outfile << recvbuf[i];
            }
            std::cout << "\n";
            return;
        }
        else if (iResult == 0) {
            outfile << "Connection Closed!" << endl;
        }
        else {
            outfile << "Recv Failed: " << WSAGetLastError() << endl;
        }
    } while (iResult > 0);
    return;
}

/**
 * Handles closing of socket connection for both sending and receiving.
 * 
 * \return int, 1 if error occured, 0 otherwise
 */
int PortScanner::close()
{
    // Shutdown the connection for sending and receiving
    iResult = shutdown(connectSocket, SD_BOTH);
    if (iResult == SOCKET_ERROR) {
        outfile << "Shutdown Failed: " << WSAGetLastError() << endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    // Cleanup resources
    closesocket(connectSocket);
    WSACleanup();
}
