#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <fstream>

#include "PortData.h"

using namespace std;

#define MIN_PORT 255			// Windows min port
#define MAX_PORT 65535			// Windows max port
#define DEFAULT_BUFLEN 256

class PortScanner {
public:
	PortScanner(const char* ip);
	~PortScanner();

	int init(int portNumber);
	int scan(int portNumber, string message);


	// Implement single thread first
	// Worry about multi threading later
	// Thread will select a port number, attempt to connect, return and store result in array

private:
	ofstream outfile;

	thread send_thread;
	vector<PortData> data;
	
	// Winsock2 Variables
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	const char* ip;
	const char* sendbuf;

	char recvbuf[DEFAULT_BUFLEN];

	int recvbuflen = DEFAULT_BUFLEN;

	int iResult;
	WSADATA wsaData;
	SOCKET connectSocket = INVALID_SOCKET;
};

