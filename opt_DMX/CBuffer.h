/* Buffer class and Buffer Maintenance Functions */

#pragma once
#define _WINSOCKAPI_

#include <string.h> 
#include <stdio.h> 
#include <iostream>
#include "WinSock2.h"
#include "Windows.h"
#include "Protocol_Defines.h"

// Default Buffer size.
#define MAX_BUF_SIZE_DEFAULT			1024
// Length Field Variable.
#define LENGTH_FIELD_VAR				1									

class CBuffer
{
private:
	// Pointer to the actual Buffer Array in CBuffer class.
	char  * aucBuffer;
	// Index variable to keep track of read location
	int readPt;
	// Index variable to keep track of write location
	int writePt;
	// Size of the Buffer.
	int buffer_size;														

public:
	// Writes an external buffer into the Buffer
	int writeToBuffer(char* extbuffer, int maxbuf);
	// Writes specified part of an external buffer into the Buffer
	int writeToBuffer(char* extbuffer, int start, int stop);
	// Read from the buffer into an external buffer.
	int readFromBuffer(char* extbuffer, int maxbuf);
	// Reads from the buffer into an external buffer without removing the conetents from the buffer.
	int readWithoutRemove(char* extbuffer, int maxbuf);
	// Returns the length of the buffer
	int getBufferLength();
	// Returns the length of the first command in the buffer.
	int getCommandLength();													

	// Mutex on the CBuffer class to prevent multiple access to the buffer at once.
	HANDLE hBufMutex;														

	// Signifies a new command in the Buffer.
	int new_cmd_flag;														
	
	// Normal constructor for default size buffer: 1024 bytes.
	CBuffer();								
	// Constructor with buffer sized passed.
	CBuffer(int passed_buf_size);								
	// Destructor
	~CBuffer();																
};