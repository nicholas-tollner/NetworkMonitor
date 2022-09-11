#include "PortData.h"

// Constructor
PortData::PortData(const int portNumber, vector<string> data)
{
	this->portNumber = portNumber;
	this->data = data;
}

// Destructor
PortData::~PortData()
{

}