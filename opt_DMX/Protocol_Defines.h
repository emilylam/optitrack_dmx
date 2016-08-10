
#pragma once

// Define statements for start byte:
#define START						(char)0xA4

// Define protocol number:
#define PROTOCOL_FRONT_MSB			(char)0xF0
#define PROTOCOL_FRONT_LSB			(char)0x00
#define PROTOCOL_FRONT_USER_MSB		PROTOCOL_FRONT_MSB
#define PROTOCOL_FRONT_USER_LSB		(char)0x01
#define PROTOCOL_TRACKER_MSB		PROTOCOL_FRONT_MSB
#define PROTOCOL_TRACKER_LSB		PROTOCOL_FRONT_LSB

#define PROTOCOL_BACK_MSB			(char)0x00
#define PROTOCOL_BACK_LSB			(char)0x00

// Define expected response type:
#define QUICK_RESPONSE		        (char)0x00
#define WAIT_RESPONSE		        (char)0x01
#define REPEATED_RESPONSE	        (char)0x02


// Destinations
#define DEFAULT_DEST				(char)0x00
#define DEFAULT_DEST_MSB			(char)0x00
#define DEFAULT_DEST_LSB			(char)0xD0

#define DEFAULT_USER_MSB			(char)0xFF
#define DEFAULT_USER_LSB			(char)0x00

#define TRACKER_MSB					(char)0xEE
#define TRACKER_LSB					(char)0xEF

// Define statements for commands:

	// General Commands:
#define OK             		        (char)0x00
#define ERR          		        (char)0x01
#define END            		        (char)0x02
#define PING	     		        (char)0x03
#define ONLINE                      (char)0x04
#define DEVS_STATUS					(char)0x05
#define LOCALIZATION				(char)0x06
#define HELLO						(char)0x07
#define ASK_IF_READY				(char)0x08
#define SYSTEM_READY				(char)0x09
#define SYSTEM_BUSY					(char)0x0A
#define LOCATION_STATUS				(char)0x0B
#define DEVS_STATUS_ALL				(char)0x0C
#define DMX							(char)0x0D

	// Color Sensor Commands:
#define SLEEP          		        (char)0xCA
#define WAKE           		        (char)0xCB
#define SAMPLE         		        (char)0xCC
#define SET_DYNAMICS				(char)0xCD
#define GET_GAIN					(char)0xCE
#define GET_PRESCALER				(char)0xCF

	// LED Controls Commands:
#define SET             	        (char)0xAA 
#define INCREASE        	        (char)0xAB
#define DECREASE        	        (char)0xAC
#define DIM             	        (char)0xAD
#define STATUS          	        (char)0xAE

// Define statements for Channel (Value A):

	// LEDS
#define ALL_LEDS        	        (char)0x3F
#define RED_LED         	        (char)0x01       
#define GREEN_LED        	        (char)0x02
#define BLUE_LED      		        (char)0x04
#define AMBER_LED       	        (char)0x08
#define COOLWHITE_LED   	        (char)0x10
#define WARMWHITE_LED   	        (char)0x20
#define INRARED         	        (char)0x40
#define NO_LEDS						(char)0x00

	// Color Sensors
#define RED_CS          	        (char)0x01
#define BLUE_CS         	        (char)0x02
#define GREEN_CS        	        (char)0x04
#define AMBER_CS        	        (char)0x08
#define ALL_CS                  	(char)0x0F
#define NO_CS						(char)0x00

// Define statements for Value B (Intensity Bytes)
#define LED_OFF						(char)0x00
#define A_FOURTH_OF_MAX		        (char)0x40
#define HALF_MAX					(char)0x80
#define THREE_FOURTH_MAX	        (char)0xC0
#define MAX_INTENSITY		        (char)0xFF

// Define statemnts for Value C (Time To Set Byte)
#define IMMEDIATELY                 (char)0x00
#define FIVE_SECONDS                (char)0x05
#define TEN_SECONDS                 (char)0x0A
#define TWENTY_SECONDS              (char)0x14
#define ONE_MINUTE					(char)0x3C
#define ONE_AND_A_HALF_MIN	        (char)0x5A
#define TWO_MINUTES					(char)0x78
#define THREE_MINUTES		        (char)0xB4
#define FOUR_MINUTES		        (char)0xF0

// Locations:
#define START_LOCATION				0
#define LENGTH_LOCATION				1
#define DESTINATION_MSB_LOCATION	2
#define DESTINATION_LSB_LOCATION	3
#define PROTOCOL_MSB_LOCATION		4
#define PROTOCOL_LSB_LOCATION		5
#define RESPONSE_LOCATION			6
#define COMMAND_LOCATION			7
#define NUM_DEVICE_LOCATION			8
#define LOC_ONOFF_LOCATION			8
#define MARKER_NUM_LOCATION			8
#define CHANNEL1_LOCATION           8
#define CHANNEL2_LOCATION           9
#define VALUE1_LOCATION            10
#define VALUE2_LOCATION            11
#define VALUE3_LOCATION            12
#define VALUE4_LOCATION            13
#define VALUE5_LOCATION            14
#define VALUE6_LOCATION            15
#define NUM_DMX_CH_LOC              8
#define DMXCHANNEL1_LOC             9
#define DMXCHANNEL2_LOC            10
#define DMXCHANNEL3_LOC            11
#define DMXCHANNEL4_LOC            12
#define DMXCHANNEL5_LOC			   13
#define DMXCHANNEL6_LOC			   14
#define DMXCHANNEL7_LOC			   15

// OTHER:
#define ADD_TO_GET_LENGTH			2
#define EMPTY_CHECKSUM				(unsigned char) 0x00
#define TIMEOUT_IN_MILISECONDS		15000 // 15 seconds
#define TIMEOUT_IN_SECONDS			15 // 15 seconds

#define FIVE_SECONDS_CHECK			5
#define CHECK_TIME					300 // 300 seconds = 5 minutes
#define EVERY_SECOND				1 // 1 second

#define PING_CMD_LENGTH				9
#define HEADER_LENGTH				8
#define STANDARD_LENGTH				HEADER_LENGTH+1
#define STD_LEN_CMD					(char) STANDARD_LENGTH-ADD_TO_GET_LENGTH
#define LED_CMD_LENGTH				18

#define NUM_OF_COORDS				3

// DEVICE INFO:
#define ARD_0_ID				(unsigned char) 0xD0
#define ARD_0_IP				"128.197.164.68"			// Nicole's Arduino	-----
#define ARD_0_PORT				"1"					// Nicole's Port	-----
#define ARD_1_ID				(unsigned char) 0xD1
#define ARD_1_IP				"128.197.164.68"			// Other Ard		-----
#define ARD_1_PORT				"2"					// Other Ard		-----

#define ALL_DEVS					(char)0xFF

#define DEV_TBL_OK					(char)0x00
#define DEV_TBL_ERR					(char)0x01

#define STATUS_UNKNOWN		        0
#define STATUS_ONLINE_RESPONSIVE	1
#define STATUS_ONLINE_UNRESPONSIVE	2
#define STATUS_OFFLINE				3

// LOCALIZATION:
#define OFF							(char) 0x00
#define ON							(char) 0xFF

#define RIGHTHANDED					(char) 0x0D
#define LEFTHANDED					(char) 0x0E

#define VERSION_1_1					(char) 0x01

#define EVERY_NUM_FRAMES			5


//  DMX

