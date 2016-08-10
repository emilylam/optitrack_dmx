# optitrack_dmx

1. opt_DMX
  * Obtains rigid body points from Optitrack using NatNet
  * Translates information and creates a client to send command to Arduino
  * TCP client/server networking
  * Has a bunch of other unnecessary code for integrating a user interface, multithreaded functionality, other device control (namely TiLED), broadcasting commands, and room status.

2. DMX
  * Acts as a TCP server and waits for commands from opt_DMX
  * Translates command into DMX command to sends to beam

3. Notes
  * Communication between opt_DMX and DMX uses a protocol defined in Protocol_Frame.pdf
  * Don't forget to generate an XOR checksum
