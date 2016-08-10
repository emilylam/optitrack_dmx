#include "CProtocol.h"
#include "Protocol.h"
#include "Protocol_Back.h"
#include <math.h>       /* atan2 */

#pragma comment(lib,"ws2_32.lib")

HANDLE CProtocol :: globalMutex;

#define TEST_LENGTH 18
#define TEST_IP "192.168.1.211" // SLURP IP
#define TEST_PORT "5000"


// Defines for Headlight position
#define IN2MT 0.0254
#define HEADLIGHT_X 0
#define HEADLIGHT_Y 95.5*IN2MT // Headlight is hung 95.5in above the floor
#define HEADLIGHT_Z
#define PI 3.14159265

CProtocol :: CProtocol() {
	//Tester class

	buffer_in = new CBuffer(); //  Into TiLED Device
	buffer_out = new CBuffer(); // Out of TiLED Device

	// Set up Default command
	cmd = new char[TEST_LENGTH];
	cmd[START_LOCATION]				= START;										// start byte
	cmd[LENGTH_LOCATION]			= (char)(TEST_LENGTH - ADD_TO_GET_LENGTH);	// command length
	cmd[DESTINATION_MSB_LOCATION]	= DEFAULT_DEST_MSB;								// dest MSB.. unused right now so default it
	cmd[DESTINATION_LSB_LOCATION]	= DEFAULT_DEST;									// this will be filled in in a sec
	cmd[PROTOCOL_MSB_LOCATION]		= PROTOCOL_BACK_MSB;							// back protocol msb
	cmd[PROTOCOL_LSB_LOCATION]		= PROTOCOL_BACK_LSB;							// back protocol lsb
	cmd[RESPONSE_LOCATION]			= QUICK_RESPONSE;								// expected response
	cmd[COMMAND_LOCATION]			= DMX;											
	cmd[NUM_DMX_CH_LOC]				= 7;
	cmd[DMXCHANNEL1_LOC]			= (char)0x00;  // Pan
	cmd[DMXCHANNEL2_LOC]			= (char)0x00;  // Tilt
	cmd[DMXCHANNEL3_LOC]			= (char)0xFF;  // Dimmer -- No flashing at FF.
	cmd[DMXCHANNEL4_LOC]			= (char)0xFF;  // Red
	cmd[DMXCHANNEL5_LOC]			= (char)0xFF;  // Green
	cmd[DMXCHANNEL6_LOC]			= (char)0xFF;  // Blue
	cmd[DMXCHANNEL7_LOC]			= (char)0xFF;  // White
	cmd[TEST_LENGTH-1]				= EMPTY_CHECKSUM;								// empty checksum

	WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
	buffer_in->writeToBuffer(cmd, TEST_LENGTH);
	ReleaseMutex(buffer_in->hBufMutex);

}

CProtocol :: CProtocol(float *x, float *y, float *z) {

	x_coord = x;
	y_coord = y;
	z_coord = z;

	printf("x: %3.2f y: %3.2f z: %3.2f\n", *x_coord, *y_coord, *z_coord);

	buffer_in = new CBuffer(); //  Into TiLED Device
	buffer_out = new CBuffer(); // Out of TiLED Device

	// Set up New command
	char * cmd = new char[TEST_LENGTH];
	cmd[START_LOCATION]				= START;										// start byte
	cmd[LENGTH_LOCATION]			= (char)(TEST_LENGTH - ADD_TO_GET_LENGTH);	// command length
	cmd[DESTINATION_MSB_LOCATION]	= DEFAULT_DEST_MSB;								// dest MSB.. unused right now so default it
	cmd[DESTINATION_LSB_LOCATION]	= DEFAULT_DEST;									// this will be filled in in a sec
	cmd[PROTOCOL_MSB_LOCATION]		= PROTOCOL_BACK_MSB;							// back protocol msb
	cmd[PROTOCOL_LSB_LOCATION]		= PROTOCOL_BACK_LSB;							// back protocol lsb
	cmd[RESPONSE_LOCATION]			= QUICK_RESPONSE;								// expected response
	cmd[COMMAND_LOCATION]			= DMX;											
	cmd[NUM_DMX_CH_LOC]				= 7;
	cmd[DMXCHANNEL1_LOC]			= (char)0x00;  // Pan
	cmd[DMXCHANNEL2_LOC]			= (char)0x00;  // Tilt
	cmd[DMXCHANNEL3_LOC]			= (char)0xFF;  // Dimmer -- No flashing at FF.
	cmd[DMXCHANNEL4_LOC]			= (char)0xFF;  // Red
	cmd[DMXCHANNEL5_LOC]			= (char)0xFF;  // Green
	cmd[DMXCHANNEL6_LOC]			= (char)0xFF;  // Blue
	cmd[DMXCHANNEL7_LOC]			= (char)0xFF;  // White
	cmd[TEST_LENGTH-1]				= EMPTY_CHECKSUM;								// empty checksum

	WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
	buffer_in->writeToBuffer(cmd, TEST_LENGTH);
	ReleaseMutex(buffer_in->hBufMutex);


}

CProtocol :: ~CProtocol() {

	delete buffer_in;
	delete buffer_out;

}

int CProtocol :: RunClient() {

		// create a pointer to our target device character in the command
		//targetDev = &command[DESTINATION_LSB_LOCATION];
		//char * targetDev = NULL;
		//char * dev = (char *) targetDev;

		// define variables
		WSADATA wsaData;
		SOCKET connectSocket = INVALID_SOCKET;

		// ------Initialize Winsock-------------------------------
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) { 
			printf("WSAStartup failed: %d\n", iResult);

			// cleanup stuff
			WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
			int cmdLength = buffer_in->getCommandLength();
			ReleaseMutex(buffer_in->hBufMutex);

			char * command = new char[cmdLength];

			WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
			buffer_in->readFromBuffer(command, cmdLength);
			// buffer_in->new_cmd_flag = 0;
			ReleaseMutex(buffer_in->hBufMutex);

			return 1;
		}


		// set up structure to hold properties of host
		struct addrinfo		*result = NULL,
							*ptr = NULL,
							hints;

		ZeroMemory( &hints, sizeof(hints) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		char * servIP = TEST_IP;
		char * servPort = TEST_PORT;

		iResult = getaddrinfo(servIP, servPort, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();

			// cleanup stuff
			WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
			int cmdLength = buffer_in->getCommandLength();
			ReleaseMutex(buffer_in->hBufMutex);

			char * command = new char[cmdLength];

			WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
			buffer_in->readFromBuffer(command, cmdLength);
			//buffer_in->new_cmd_flag = 0;
			ReleaseMutex(buffer_in->hBufMutex);

			return 1;
		}

		// Attempt to connect to the first address returned by
		// the call to getaddrinfo
		ptr=result;


		// ------Create a SOCKET----------------------------------
		// socket()
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();

			// cleanup stuff
			WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
			int cmdLength = buffer_in->getCommandLength();
			ReleaseMutex(buffer_in->hBufMutex);

			char * command = new char[cmdLength];

			WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
			buffer_in->readFromBuffer(command, cmdLength);
			//buffer_in->new_cmd_flag = 0;
			ReleaseMutex(buffer_in->hBufMutex);
			return 1;
		}

		// Connect to server.
		// connect()
		iResult = connect( connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
		}

		freeaddrinfo(result);		// free space, we connected so we don't this anymore

		if (connectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");

			WSACleanup();


			// cleanup stuff
			WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
			int cmdLength = buffer_in->getCommandLength();
			ReleaseMutex(buffer_in->hBufMutex);

			char * command = new char[cmdLength];


			WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
			buffer_in->readFromBuffer(command, cmdLength);
			// buffer_in->new_cmd_flag = 0;
			ReleaseMutex(buffer_in->hBufMutex);

			return 1;
		}

		// Communicate with the server
		Protocol_Back * protBack = new Protocol_Back(buffer_in,buffer_out);
		protBack->Talk(connectSocket);
	

		// cleanup
		closesocket(connectSocket);
		WSACleanup();

		// Get new data and write to buffer
		printf("x: %3.2f y: %3.2f z: %3.2f\n", *x_coord, *y_coord, *z_coord);

		findTilt();
		findPan();

		WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
		buffer_in->writeToBuffer(cmd, TEST_LENGTH);
		ReleaseMutex(buffer_in->hBufMutex);

		return 0;
	}



int CProtocol :: findTilt() {
	double tilt;
	char tilt_;
	tilt = atan2f((HEADLIGHT_Y - *y_coord), sqrtf( pow(*x_coord,2) + pow(*z_coord, 2) ))  * 180 / PI;
	tilt_ = floor( (tilt/360)*255 );
	cmd[DMXCHANNEL2_LOC] = tilt_;  // Tilt
	return 0;
}

int CProtocol :: findPan() {
	double pand;
	char pand_;
	if (*x_coord < 0) {
		pand = -atan2f(*x_coord,*z_coord) * 180 / PI;
	}
	else if (*x_coord > 0) {
		pand = 360 - (atan2f(*x_coord,*z_coord) * 180 / PI );
	}
	else
		pand = atan2f(*x_coord, *z_coord) * 180 / PI;
	pand_ = floor( (pand/360)*255);
	cmd[DMXCHANNEL1_LOC] = pand_;  // Pan
	return 0;
}