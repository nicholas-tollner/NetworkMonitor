#include "PortScanner.h"

#define SKIP 3

/* Need to create multiple threads
	Each thread must attempt to connect to a port number
	
*/

// Constructor
PortScanner::PortScanner(const char* ip)
{
    this->ip = ip;

    outfile.open("debug.txt");

    outfile << " === Application Started === " << endl;
}

// Destructor
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
    iResult = getaddrinfo(ip, std::to_string(portNumber).c_str(), &hints, &result);
    if (iResult != 0) {
        outfile << "Fail: " << iResult << endl;
        WSACleanup();
        return 1;
    }
    outfile << "Success: " << iResult << endl;
}

// Multithreading for sending and receiving
int PortScanner::scan(int portNumber, string message)
{
    // Point to result addrinfo struct
    ptr = result;

    // Setup socket to use for connection to server
    connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

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

    thread send_thread([this, portNumber, message] { sendData(portNumber, message); });
    thread recv_thread([this] { recvData(); });

    send_thread.join();
    recv_thread.join();

    return 0;
}

int PortScanner::sendData(int portNumber, string message)
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
}

int PortScanner::recvData()
{
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();         // Start time
    do {
        // Receive data then return
        iResult = recv(connectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            for (int i = 0; i < iResult; i++) {
                outfile << recvbuf[i];
            }
            std::cout << "\n";
            return 0;
        }
        else if (iResult == 0) {
            outfile << "Connection Closed!" << endl;
        }
        else {
            outfile << "recv failed: " << WSAGetLastError() << endl;
        }
    } while (iResult > 0 && std::chrono::steady_clock::now() - start <= std::chrono::seconds(5));
    return 0;
}
