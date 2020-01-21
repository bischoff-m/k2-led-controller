// ########################## Unit test for DmxSimple ##########################

#include "DmxSimple-modified.h"
#include <ArduinoJson.h>

void setup() {
  DmxSimple.usePin(3);
  
  int data[7][4];
  for(int i = 0; i < 7; i++) {
    data[i][0] = 200;
    data[i][1] = 0;
    data[i][2] = 0;
    data[i][3] = 200;
  }

  // data[0][0] = 200;
  // data[0][1] = 200;
  // data[0][2] = 200;
  // data[0][3] = 200;
  
  int address_counter = 1;

  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 4; j++) {
      int value = data[i][j];
      DmxSimple.writeToBuffer(address_counter, value);
      address_counter++;
    }
  }
  DmxSimple.sendData();
}


void loop() {

}