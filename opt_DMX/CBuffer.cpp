/* Buffer class and Buffer Maintanence Functions */

#include "CBuffer.h" 


// Constructor for default buffer size.
CBuffer::CBuffer() : aucBuffer(new char[MAX_BUF_SIZE_DEFAULT]) // Create new Buffer with default size.
	
{
	// Initialize buffer_size, readPt, and writePt.
	buffer_size = MAX_BUF_SIZE_DEFAULT;
	readPt = 0;
	writePt = 0;

	// Zero memory.
	ZeroMemory(aucBuffer, MAX_BUF_SIZE_DEFAULT);

	// Set new command flag to zero.
	new_cmd_flag = 0;

	// Create a Mutex for the Buffer
	hBufMutex = CreateMutex(NULL, FALSE, NULL);
	if (hBufMutex == NULL) 
		printf("Error creating buffer mutex\n");

}

// Constructor for passed buffer size
CBuffer::CBuffer(int passed_buf_size) : aucBuffer(new char[passed_buf_size]) // Create new Buffer with specified size.

{
	// Initialize buffer_size, readPt, and writePt.
	buffer_size = passed_buf_size;
	readPt = 0;
	writePt = 0;

	// Zero memory.
	ZeroMemory(aucBuffer, passed_buf_size);

	// Set new command flag to zero.
	new_cmd_flag = 0;

	// Create a Mutex for the Buffer
	hBufMutex = CreateMutex(NULL, FALSE, NULL);
	if (hBufMutex == NULL) 
		printf("Error creating buffer mutex\n");

}

// Destructor
CBuffer::~CBuffer() 
{
	// Delete Buffer pointer.
	delete aucBuffer;
}

// Returns the length of the buffer
int CBuffer::getBufferLength() 
{
	int len = 0;
	if(writePt >= readPt)
	{
		len = writePt - readPt;
	}
	else
	{
		// accounts for looping buffer
		len = buffer_size - readPt + writePt;
	}
	return len;
}

// Returns the length of the first command in the buffer.
int CBuffer::getCommandLength()
{
	/* Length is read in from command, which doesn't include START and LENGTH byte. 
	So add two to get actual length. */

	int len = 0;

	// If readPt is at the end of the Buffer
	if(readPt == MAX_BUF_SIZE_DEFAULT - 1 && writePt != readPt)
	{
		len = aucBuffer[0] + ADD_TO_GET_LENGTH;
	}
	// else
	else if (writePt != readPt)
	{
		len = aucBuffer[readPt + LENGTH_FIELD_VAR] + ADD_TO_GET_LENGTH;
	}

	return len;
}


// Read from the buffer into an external buffer.
int CBuffer::readFromBuffer(char* extbuffer, int MAXBUF)
{
	int i = 0;
	int MAXDATA = this->getBufferLength();
	for(i=0;(i<MAXDATA) && (i<MAXBUF);i++) // For i<Shortest buffer length
	{
		extbuffer[i] = aucBuffer[readPt++];
		if(readPt == buffer_size)
		{
			readPt = 0; // Loops to beginning of buffer
		}
	}
	return i;
}

// Reads from the buffer into an external buffer without removing the conetents from the buffer. 
int CBuffer :: readWithoutRemove(char * extbuffer, int maxbuf) {

	int i = 0;
	int maxdata = this->getBufferLength();
	int newPt = readPt;

	for (i = 0; (i < maxdata) && (i < maxbuf); i++) {
		extbuffer[i] = aucBuffer[newPt++];
		if (newPt == buffer_size)
			newPt = 0;
	}
	return i;
}
		
// Writes an external buffer into the Buffer
int CBuffer::writeToBuffer(char* extbuffer, int maxbuf)
{	
	// Calls the overloaded version of writeToBuffer with 0 as start variable and the max buf size as the stop variable
	return writeToBuffer(extbuffer, 0, maxbuf-1); 
}


// Writes specified part of an external buffer into the Buffer
int CBuffer::writeToBuffer(char* extbuffer, int start, int stop)
{
	int i=0;
	int maxbuf = stop-start+1;

	for(i=0;(i<maxbuf) && (i<buffer_size);i++) // For i<Shortest buffer length
	{
		aucBuffer[writePt++] = extbuffer[start+i];
		if(writePt == buffer_size)
		{
			writePt = 0; // Loops to beginning of buffer
		}
	}
	return 0;
}
