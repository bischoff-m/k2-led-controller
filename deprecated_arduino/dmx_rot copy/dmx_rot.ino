/* This program allows you to set DMX channels over the serial port.
**
** After uploading to Arduino, switch to Serial Monitor and set the baud rate
** to 9600. You can then set DMX channels using these commands:
**
** <number>c : Select DMX channel
** <number>v : Set DMX channel to new value
**
** These can be combined. For example:
** 100c355w : Set channel 100 to value 255.
**
** For more details, and compatible Processing sketch,
** visit http://code.google.com/p/tinkerit/wiki/SerialToDmx
**
** Help and support: http://groups.google.com/group/dmxsimple       */

#include <DmxSimple.h>

void setup() {
  DmxSimple.usePin(3);
  DmxSimple.write(1, 0);
  DmxSimple.write(2, 255);
  DmxSimple.write(3, 255);
  DmxSimple.write(4, 0);

  DmxSimple.write(5, 0);
  DmxSimple.write(6, 0);
  DmxSimple.write(7, 255);
  DmxSimple.write(8, 0);

  DmxSimple.write(9, 170);
  DmxSimple.write(10, 0);
  DmxSimple.write(11, 0);
  DmxSimple.write(12, 170);

  DmxSimple.write(13, 170);
  DmxSimple.write(14, 170);
  DmxSimple.write(15, 100);
  DmxSimple.write(16, 170);

  DmxSimple.write(17, 170);
  DmxSimple.write(18, 0);
  DmxSimple.write(19, 0);
  DmxSimple.write(20, 170);

  DmxSimple.write(21, 170);
  DmxSimple.write(22, 0);
  DmxSimple.write(23, 0);
  DmxSimple.write(24, 170);

  DmxSimple.write(25, 170);
  DmxSimple.write(26, 0);
  DmxSimple.write(27, 0);
  DmxSimple.write(28, 170);
}

void loop() {

}
