# optitrack_dmx

1. opt_DMX
  a. Obtains rigid body points from Optitrack using NatNet
  b. Translates information and creates a client to send command to Arduino
  c. TCP client/server networking
  d. Has a bunch of other unnecessary code for integrating a user interface, multithreaded functionality, other device control (namely TiLED), broadcasting commands, and room status.

2. DMX
  a. Acts as a TCP server and waits for commands from opt_DMX
  b. Translates command into DMX command to sends to beam

3. Notes
  a. Communication between opt_DMX and DMX uses a protocol defined in Protocol_Frame.pdf
  b. Don't forget to generate an XOR checksum
