#pragma once

#define _WINSOCKAPI_

#include "WinSock2.h"
#include "Windows.h"


class Protocol {
public: 
	Protocol();
	~Protocol();
	virtual void Talk(SOCKET);


private:

	
	


protected:
};