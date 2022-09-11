#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#include "PortData.h"

using namespace std;

#define MIN_PORT = 255;			// Windows min port
#define MAX_PORT = 65535;		// Windows max port

class PortScanner {
public:
	PortScanner();
	~PortScanner();

	void scan();

	// Implement single thread first
	// Worry about multi threading later
	// Thread will select a port number, attempt to connect, return and store result in array

private:
	thread send_thread;
	PortData* info[];

};

