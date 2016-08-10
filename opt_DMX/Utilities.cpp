/* The Utilities files were designed to include functions that would be called throughout the entire system server. */

#pragma once

#include "Utilities.h"


///////////////////////////////////////////////////////////////////////////////////////////
// Function Defintions
///////////////////////////////////////////////////////////////////////////////////////////

// Creates an xor checksum and places it in the last byte of the passed array.
void create_checksum(char* sequence)
{
	// The length here is the length of the sequence not including the checksum byte
	char length = sequence[LENGTH_LOCATION] + LENGTH_LOCATION; 

	// Calculates checksum using a bitwise xor
	char result_of_xor = 0x00;
	for (int i = 0; i < length; i++) {
		result_of_xor ^= sequence[i];
	}

	// Stores calculated checksum in the checksum byte location, last byte of the passed array.
	sequence[length] = result_of_xor;
	return;
}


// Calculates checksum and determines whether it matches the checksum at the end of the array or not.
char check_checksum (char* sequence)
{
	// The length here is the length of the sequence not including the checksum byte
	char length = sequence[LENGTH_LOCATION] + LENGTH_LOCATION;

	// Calculates checksum using a bitwise xor
	char result_of_xor = 0x00; 
	for (int i = 0; i < length; i++) {
		result_of_xor ^= sequence[i];
	}

	// Compares the calculated xor to the last byte in the array.
	if (result_of_xor != sequence[length])
	{
		return ERR; // Corrupted array. Return ERR.
	}
	else
	{
		return OK; // Valid array. Return OK.
	}
}
