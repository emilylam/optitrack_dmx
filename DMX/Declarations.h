// For debugging: -- comment out for release version
#define DEBUG 1
 
// MAC address for ethernet shield from sticker on shield.
byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x9D, 0x19}; // Ethernet Shield T5E; 
IPAddress ip; // IP address of the server that the Arduino will set up.
// Arduino Server Port: 
int port_s = 5000;
//Initialize Ethernet server library:
EthernetServer server(port_s);

// DMX defintions:
int numDMXChannels = 7;
int numDXMChannels_new;
byte DMX_value[20]; // Channel Array, declared to 20 channels for now.

// Function prototypes:
void setDefaultState();
void checkForMessage();
void implementDMX();
void writeToClient(byte message[], int sizeOfMessage);
boolean checkCheckSum();
byte generateCheckSum(byte buffer[], int length); // Generates check sum to send to client
byte checkVersion1(byte version1);
byte checkVersion2(byte version2);
void decodeCommand(byte buffer[]);
 
// Delay bariable to display Arduino IP address:
unsigned long setupTime;
 
// Index variable to keep track of size of message from client:
int i=0;
 
// Flags:
boolean fIp=false;
boolean fServer = true; //True if Arduino acting in server mode. False if Arduino acting in client mode.
boolean fGotMessage = false; //True if Arduino received a new message from client
boolean fCheck= false; //True is check sum is valid
boolean fFirstOk=false;
boolean fPing=false;
boolean fDecode=false;
boolean fDecoded=false;
boolean fStatus=false;
 
// Variables for Protocol format:
char thisChar; // characters read from client
byte charToByte; // characters converted to bytes
byte inputBytes[256]; //buffer to store sequence from the Server (as bytes) 
byte protocol1=0; 
byte protocol2=0;
byte sum=0;
byte sendSum=0;
byte timeDuration= 0;
byte checkSum= 0;
