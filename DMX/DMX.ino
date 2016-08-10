#include <SPI.h> 
#include <Ethernet.h> 
#include "Protocol_Defines.h" 
#include "Declarations.h"
#include "DmxMaster.h"

// SETUP LOOP //////////////////////////////////////////////////////////////////////////////////////////
void setup() { 
 
 // Set up DMX
 DmxMaster.usePin(3); // Default output pin
 DmxMaster.maxChannel(numDMXChannels); // Default Maximum channel on DMX device
 
 //Start Ethernet connection: 
 Ethernet.begin(mac); 
 // Start listening for clients: 
 server.begin(); 
 // Obtain the local IP address of the current Ethernet Sheild: 
 ip = Ethernet.localIP(); 
  
 //Only do this if in debug mode 
 #ifdef DEBUG  
 // Open serial communications and wait for port to open: 
 Serial.begin(9600); 
 // After first reset, Serial is set to 0 and never changes to 1 so it gets stuck in this while loop 
 while (!Serial) { 
   ; // wait for serial port to connect. Needed for Leonardo only. 
 } 
 #endif 
  
 // Set initial time, used in debug mode for displaying IP address to Serial Monitor: 
 setupTime=millis(); 
 fIp=true; 
   
 // Calls default state function to set LEDs to their default intensity: 
 // setDefaultState(); Not implemented yet
   
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////

// MAIN LOOP /////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // Checks for new messages from client through Ethernet shield: 
   checkForMessage(); 
    
  // Prints out Arduino's local IP address to Serial Monitor. 
  // Only implemented in debug mode. Can't print from setup. Delay of 2 seconds needed to give enough time to open Serial Monitor to see IP address. 
  #ifdef DEBUG 
  if(millis()- setupTime > 3000){ 
    if(fIp){ 
      for (byte thisByte = 0; thisByte < 4; thisByte++) { 
       // Print the value of each byte of the IP address: 
       Serial.print(ip[thisByte], DEC); 
       Serial.print(".");  
      } 
      fIp=false; 
    } 
  } 
  #endif 
  
  // Calls specified functions based on flags set in the call command function: 
  if (fGotMessage){      
    // If message has been decoded  
    if(fDecoded==1) { 
      implementDMX();
      Serial.print("Done");
    } 
  } 
     
  else if(fGotMessage==0){ 
    // Need to reset this flag so DMX control are only called when needed: 
    fDecode=0; 
  } 
    
} 

//////////////////////////////////////////////////////////////////////////////////////////////////////

// MAIN FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////  
void checkForMessage(){ 
   // Returns a Client object; if no Client has data available for reading, this object will evaluate to false in an if-statement 
   EthernetClient client =  server.available(); 
     
    // Indicates if the specified Ethernet client is ready: 
    if (client) { 
        // Resets buffer index when new message comes in: 
        i=0; 
        #ifdef DEBUG 
        Serial.println("Gets inside if client"); 
        Serial.println("Bytes available:"); 
        Serial.println(client.available()); 
        Serial.println("end"); 
        #endif 
          
        // If the client is connected, read in any available messages: 
        while (client.connected()) { 
          if(client.available()){ 
            #ifdef DEBUG 
            Serial.println("Gets to client. available"); // Does not get to this point then connection between Arduino and client fails 
            #endif 
            // Reads each character sent from client, converts to byte and stores in byte buffer: 
            thisChar = client.read(); 
            charToByte = (byte)thisChar; 
            inputBytes[i] = charToByte; 
            
            
            //Checks the first byte sent from client: 
            if(i==START_LOCATION){ 
                Serial.println("TEST");
                Serial.println(i);
                Serial.println(inputBytes[i]);
                if(inputBytes[i]==START){ 
                    #ifdef DEBUG 
                    Serial.println("GOOD START"); 
                    #endif 
                    // Arduino has received a message with a valid start byte: 
                    fGotMessage=true; 
                } 
                else{ 
                    // Arduino has not recieved a valid start byte: 
                    fGotMessage=false;  
                    #ifdef DEBUG 
                    Serial.println("ERROR BAD START"); 
                    Serial.println (charToByte); 
                    #endif 
                } 
            }
              
            // Increase index to read next byte: 
            i++; 
          } 
          else { // No bytes available to read from client 
            // Break out of while loop: 
            break; 
          } 
        } 
          
        // Calls check sum function to check that message read from client is not corrupt: 
        checkCheckSum();  
          
        #ifdef DEBUG 
        // Print out 1 if check sum is valid: 
        Serial.println(fCheck); 
        // Print out message from client: 
        Serial.println("Sequence from System Server:"); 
        for (int j=0; j<i; j++) { 
            Serial.println(inputBytes[j],HEX); // Prints out sequence from Server 
        } 
        #endif 
          
        // If Arduino has received message from client, need to set flags, set status variables, and send the specified messages (ping/acknowlegement/status query) back to the client: 
        if(fGotMessage && fCheck){ 
          // Will sent wait response OK message to client as long as message in is NOT a ping        
          if(inputBytes[COMMAND_LOCATION] == PING) { 
            fPing=true;  
          } 
          // LED Control Commands: 
          if(inputBytes[COMMAND_LOCATION] == DMX) { 
            fFirstOk=true; // Sets flag to send first OK message to client once message has been received 
            fDecode=true; //  Sets flag for decode command function             
          } 
          if(inputBytes[COMMAND_LOCATION]==STATUS){ 
            fStatus=true; // Sets flag to send status message to client 
          } 
            
          // Determines protocol versions: 
          // (Needed for messages sent back to client) 
          checkVersion1(inputBytes[4]); 
          checkVersion2(inputBytes[5]); 
            
          // Send quick response OK message when Client pings Arduino: 
          if(fPing){ 
              #ifdef DEBUG 
              Serial.println("Gets to ping"); 
              #endif 
              //Send quick response OK if client pings Arduino 
              byte pingOk[]={START, 0x06, 0x00, 0x00, protocol1, protocol2, QUICK_RESPONSE, OK}; 
              generateCheckSum(pingOk,8); 
              byte ping[]={START, 0x06, 0x00, 0x00, protocol1, protocol2, QUICK_RESPONSE, OK, sendSum}; 
              writeToClient(ping, 9); 
              fPing=false; 
          } 
            
          // Send acknowledgment OK to client after (non-ping) message is read: 
          if(fFirstOk) { 
              #ifdef DEBUG 
              Serial.println("First OK"); 
              #endif 
              // Send ok message to client once sequence has been received: 
              byte firstOk[] = {START, 0x06, 0x00, 0x00, protocol1, protocol2, WAIT_RESPONSE, OK}; 
              //server.write(firstOk, 8); 
              generateCheckSum(firstOk,8); 
              byte ok1[]={START, 0x06, 0x00, 0x00, protocol1, protocol2, WAIT_RESPONSE, OK, sendSum}; 
              writeToClient(ok1, 9); 
              fFirstOk=false; 
          } 
            
          // Checks if System server has asked Arduino for a status update. 
          // Calls status function and writes data back to the client. 
          if(fStatus){ 
              // Deal with this later
          } 
            
          // This function decodes which DMX control are being sent: 
          if(fDecode){
            #ifdef DEBUG 
            Serial.println("Gets to decode"); 
            #endif 
            decodeCommand(inputBytes); 
            fDecode=false;  
          } 
            
        } 
        else if(fCheck==0){ 
           // Reset message flag to false because message from client is corrupt: 
           fGotMessage=false; 
          //Send error message to client if check sum is invalid: 
           byte errorMessage[]={START, 0x06, 0x00, 0x00, protocol1, protocol2, QUICK_RESPONSE, ERR}; 
           generateCheckSum(errorMessage, 8); 
           byte error[]= {START, 0x06, 0x00, 0x00, protocol1, protocol2, QUICK_RESPONSE, ERR, sendSum}; 
           writeToClient(error, 9);   
        } 
          
          
      // Close connection and flush buffer: 
      client.stop();  
      client.flush(); 
        
    } 
  
}  


// Decodes sequence from Server into variables that are used for the LED controls 
void decodeCommand(byte buffer[]){ 
  
  #ifdef DEBUG 
  Serial.println("Decoding Message"); 
  #endif
  
  numDXMChannels_new = buffer[NUM_DMX_CH_LOC];
  
  // Fill in DMX_value array
  int g = 0;
  for(int k = DMXCHANNEL1_LOC; k < numDXMChannels_new+DMXCHANNEL1_LOC; k++) {
      DMX_value[g] = buffer[k];
      g++;
  }  
  
  #ifdef DEBUG  
  // Print out message from client: 
  Serial.println("DMX Values:"); 
  for (int j=0; j<7; j++) { 
      Serial.println(DMX_value[j],HEX); // Prints out sequence from Server 
  } 
  #endif 
  
  // Message is decoded, set flag to true
  fDecoded = 1;
    
  return; 
}

// Implement DMX function
void implementDMX() {
  
  #ifdef DEBUG 
  Serial.println("Implementing DMX"); 
  #endif
  
  if (numDMXChannels != numDXMChannels_new) {
      numDMXChannels = numDXMChannels_new;
      DmxMaster.maxChannel(numDMXChannels);
  }    
  
  for(int k = 0; k < numDMXChannels; k++) {
      DmxMaster.write(k+1, DMX_value[k]);
  }
  
  fDecoded==0;
  fGotMessage=false; 
  
}

////////// UTILITIES /////////////////////////////////////////////////////////////////////////////////  
  
// Calculates check sum using XOR of all bytes to be compared to check sum sent in from client: 
boolean checkCheckSum(){ 
   
  for (int j=0; j<i-1; j++){ 
    sum ^=inputBytes[j]; 
  } 
  byte check = inputBytes[inputBytes[1] +1]; //This is correct 
  
  if(sum==check){ 
    fCheck=true; 
    sum=0; //Need to reset the sum for new sequences 
    return fCheck; 
  } 
  else{ 
    fCheck=false; 
    sum=0; //Need to reset the sum for new sequences 
    return fCheck; 
  } 
} 
  
// Generates check sum byte to send to client: 
byte generateCheckSum(byte buffer[], int length){  
  sendSum=0; 
  for(int j=0; j<length; j++){ 
    sendSum^= buffer[j]; 
  } 
  return sendSum; 
} 
  
//Determines the Protocol Version (byte 1) 
byte checkVersion1(byte version1){ 
   switch (version1){ 
   case PROTOCOL_BACK_MSB: 
     protocol1=PROTOCOL_BACK_MSB ; 
     break; 
   } 
  //Add more version when necessary   
  return protocol1; 
} 
//Determines the Protocol Version (byte 2) 
byte checkVersion2(byte version2){ 
   switch (version2){ 
   case PROTOCOL_BACK_LSB: 
     protocol2=PROTOCOL_BACK_LSB; 
     break; 
   } 
  //Add more version when necessary   
  return protocol2; 
} 

  
// Writes from Arduino to client 
void writeToClient(byte message[], int sizeOfMessage){ 
  server.write(message, sizeOfMessage);   
  return; 
} 
  
void setDefaultState(){ 
  
 // Empty for now. . . 
  
 return; 
  
}
  
  
  
  
