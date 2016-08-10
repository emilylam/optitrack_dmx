#pragma once

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <IPHlpApi.h>

#include "CBuffer.h"

class CProtocol {

public:
	// Constructors and Destructors
	CProtocol();
	~CProtocol();
	CProtocol(float *x, float *y, float*z);

	// Command
	char * cmd;

	// Create pointers to buffers:
	CBuffer *buffer_in;
	CBuffer *buffer_out;

	// Create pointers to coordinates
	float *x_coord, *y_coord, *z_coord;

	int RunClient();
	int findPan();
	int findTilt();

private:

	static HANDLE globalMutex;

protected:


};