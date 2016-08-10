/* The Utilities files were designed to include functions that would be called throughout the entire system server. */

#pragma once

#include <iostream>
#include <string.h> 
#include <stdio.h> 
#include "Protocol_Defines.h"

using namespace std;

// Function Prototypes:

// Creates an xor checksum and places it in the last byte of the passed array.
void create_checksum(char* sequence);

// Verifies the checksum. Returns OK for valid checksum and ERR for invalid checksum.
char check_checksum(char* sequence);			

