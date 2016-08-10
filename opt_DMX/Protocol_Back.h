#pragma once

#include "Protocol.h"
#include "Protocol_Defines.h"
#include "Cbuffer.h"
#include <stdio.h>
#include <time.h>

#define BUFLEN 256

class Protocol_Back : public Protocol {

public:
	Protocol_Back();
	Protocol_Back(CBuffer * in, CBuffer * out);
	~Protocol_Back();
	virtual void Talk(SOCKET);



private:

	
	CBuffer * buffer_in;
	CBuffer * buffer_out;


protected:

};