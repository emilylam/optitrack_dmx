#include "Protocol_Back.h"
#include "Utilities.h"

Protocol_Back :: Protocol_Back() {

}


Protocol_Back :: Protocol_Back(CBuffer * passedbuf_in, CBuffer * passedbuf_out) {

	buffer_in = passedbuf_in;
	buffer_out = passedbuf_out;

}

Protocol_Back :: ~Protocol_Back() {

}


void Protocol_Back :: Talk(SOCKET connectSocket) {

	// Read in Command
	WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
	int cmdLength = buffer_in->getCommandLength();
	ReleaseMutex(buffer_in->hBufMutex);

	char * command = new char[cmdLength];

	WaitForSingleObject(buffer_in->hBufMutex, INFINITE);
	buffer_in->readFromBuffer(command, cmdLength);
	// buffer_in->new_cmd_flag = 0;
	ReleaseMutex(buffer_in->hBufMutex);

	// CREATE CHECKSUM =]
	create_checksum(command);

	// Store this arduino's identification
	char this_device_0 = command[DESTINATION_MSB_LOCATION];
	char this_device_1 = command[DESTINATION_LSB_LOCATION];

	// Create and Set Command Flags
	bool ping_flag = false;
	bool LED_control_flag = false;
	bool status_flag = false;
	bool dmx_flag = false;

	// Backend from the summer, most of this code is irrelevant to DMX and is commented
	if (command[COMMAND_LOCATION] == PING) {
		ping_flag = true;
	}
	else if (command[COMMAND_LOCATION] == SET || command[COMMAND_LOCATION] == INCREASE || command[COMMAND_LOCATION] == DECREASE) {
		LED_control_flag = true;
	}
	else if (command[COMMAND_LOCATION] == STATUS) {
		status_flag = true;
	}
	// THIS is the DMX piece
	else if (command[COMMAND_LOCATION] == DMX) {
		dmx_flag = true;
	}
	else {
		printf("Illegal Command. I'm lost.\n");
	}

	// Initialize variables for receiving/sending information.
	char recvbuf[BUFLEN];
	int result;
	int recvbuflen = BUFLEN;

	for (int i = 0; i < cmdLength; i++) {
		printf("%X ", command[i]);
	} // Printing so we can see the command. 


	// Send data (command)
	result = send(connectSocket, command, cmdLength, 0);
	if (result == SOCKET_ERROR) {
	    printf("send failed: %d\n", WSAGetLastError());
	    closesocket(connectSocket);
	    WSACleanup();
	}
	printf("Bytes Sent: %ld\n", result);
	
	
	// Receive data (acknowledge)
	result = recv(connectSocket, recvbuf, recvbuflen, 0);
	if (result > 0) {
		printf("Bytes received: %d\n", result);
		printf("Message:\n", recvbuf);
	}
	else
		printf("recv failed: %d\n", WSAGetLastError());


	for (int i = 0; i < result; i++) {
		printf("%X\n", (unsigned char)recvbuf[i]);
	}

	ZeroMemory(recvbuf,recvbuflen); // zeros buffer.

	// Check Flags
	
	/////////////// Deals with DMX	////////////////////////////////////

	// No DMX Backcode yet besides the acknowledge

	////////////////////////////////////////////////////////////////////

	/* Remove only this if you want the rest of the functionality.

	/////////////// Deals with Pinging /////////////////////////////////
	if (ping_flag) {
		if (result > 0) {		
			WaitForSingleObject(dev_tbl->hDTMutex, INFINITE);
			dev_tbl->SetStatusByID(this_device_1, STATUS_ONLINE_RESPONSIVE);
			ReleaseMutex(dev_tbl->hDTMutex);
			ping_flag = false;
		}
		else {
			WaitForSingleObject(dev_tbl->hDTMutex, INFINITE);
			dev_tbl->SetStatusByID(this_device_1, STATUS_ONLINE_UNRESPONSIVE);
			ReleaseMutex(dev_tbl->hDTMutex);
			ping_flag = false;
		}
	}
	////////////////////////////////////////////////////////////////////

	/////////////// Deals with Status //////////////////////////////////
	if (status_flag) {
		// Receive Status
		result = recv(connectSocket, recvbuf, recvbuflen, 0);
		if (result > 0) {
			printf("Bytes received: %d\n", result);
			printf("Message:\n%s\n", recvbuf);

			// Write status to the TiLED out buffer if checksum matches
			//if (check_checksum(recvbuf) == OK) {
				printf("helllllo");
				WaitForSingleObject(buffer_TiLED_out->hBufMutex, INFINITE);
				buffer_TiLED_out->writeToBuffer(recvbuf, result);
				ReleaseMutex(buffer_TiLED_out->hBufMutex);
			//}
			//else {
				//printf("\n\n");
				//printf("Corrupted Data Error\n");
			//}
			ZeroMemory(recvbuf,recvbuflen); // zeros buffer.
			status_flag = false;
		}

		else {
			printf("recv failed: %d\n", WSAGetLastError());
			status_flag = false;
		}

		for (int i = 0; i < result; i++) {
			printf("%X\n", (unsigned char)recvbuf[i]);
		}
	}
	////////////////////////////////////////////////////////////////////

	/*		temporarily commented out until nicole can deal with this
	/////////////// Deals with LED Controls ////////////////////////////
	if (LED_control_flag) {
		// Receive Acknowledgment when LEDs control commands have been implemented.
		result = recv(connectSocket, recvbuf, recvbuflen, 0);
		if (result > 0) {
			printf("Bytes received: %d\n", result);
			printf("Message:\n%s\n", recvbuf);

			// Write status to the TiLED out buffer if checksum matches
			//if (check_checksum(recvbuf) == OK) {
				printf("LEDs commands implemented");
			//}
			//else {
				//printf("\n\n");
				//printf("Corrupted Data Error\n");
			//}
			ZeroMemory(recvbuf,recvbuflen); // zeros buffer.
			LED_control_flag = false;
		}

		else {
			printf("recv failed: %d\n", WSAGetLastError());
			LED_control_flag = false;
		}
	}
	////////////////////////////////////////////////////////////////////
	*/

	
	printf("OUT\n");
}