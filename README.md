# optitrack_dmx

1. opt_DMX (System Server, run on a PC)
  * Obtains rigid body points from Optitrack using [NatNet](http://www.optitrack.com/products/natnet-sdk/)
  * Translates information and creates a client to send command to Arduino
  * TCP client/server networking
  * Has a bunch of other unnecessary code for integrating a user interface, multithreaded functionality, other device control (namely TiLED), broadcasting commands, and room status.

2. DMX (Beam controller, run on Arduino)
  * Acts as a TCP server and waits for commands from opt_DMX
  * Translates command into DMX command to sends to beam

3. Notes
  * [Demo Video](https://drive.google.com/file/d/0B-AcdukVhfIFREg1MzhXZ192SGs/view?usp=sharing)
  * Communication between opt_DMX and DMX uses a protocol defined in Protocol_Frame.pdf
  * Don't forget to generate an XOR checksum
  * DMX Shield and library by [TinkerKit](https://github.com/TinkerKit/DmxMaster)

Primary Contributors: Emily Lam, Johnny Glynn, Yuting Zhang

[Multimedia Communications Lab](http://hulk.bu.edu), led by Professor Thomas D.C. Little, Boston University

This work is supported by the NSF under cooperative agreement EEC-0812056 and by New York State under NYSTAR contract C090145. Any opinions, findings, and conclusions or recommendations expressed in this material are those of the author(s) and do not necessarily reflect the views of the National Science Foundation.
