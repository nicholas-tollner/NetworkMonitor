#pragma once

#include <string>
#include <vector>

using namespace std;

// Represents returned port information

class PortData {
public:
	PortData(const int portNumber, vector<string> data);
	~PortData();

private:
	int portNumber;
	vector<string> data;
};