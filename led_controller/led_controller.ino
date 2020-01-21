/**
 * This program uses DmxSimple to contol LEDs at K2 Bar.
 * It sends multiple values v_i with 0 <= v_i <= 255 to the corresponding
 * channel i with 0 <= i < n (n is number of channels)
 * 
 * Communication:
 *  - Via Serial with Baudrate 9600
 *  - Each character transmission triggers SerialEvent() which triggers
 *    update in loop() when whole transmission is complete (ends with \n)
 *  - Syntax:
 *    o Input: "v_1,v_2,...,v_n" (Example: 255,120,0,250)
 *    o Output: sends value v_i to channel/address i via DMX
 *    o Use a maximum of 600 Bytes (hardcoded in setup())
 * 
 * How to write to LED output:
 *  - Use DmxSimple.writeToBuffer(i, v_i) to prepare writing value v_i to channel i
 *  - Use DmxSimple.sendData() to actually transmit prepared values to output DMX channels
 *  - The "DmxSimple-modified" Library uses interrupts to send data via DMX. It is modified
 *    to send only one signal, so this will only work for the LEDs at K2 Bar.
 *    This is done to prevent the light from flickering, as it does, when constantly
 *    sending the signal like standard DMX control does
 */

#include "DmxSimple-modified.h"

#define PRINT_DEBUG_ENABLED false

// data input string
String data = "";

// whether data transmission is complete
bool dataComplete = false;


void setup() {
  // initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  // reserve 600 bytes for data:
  data.reserve(600);
  DmxSimple.usePin(3);

  // this is initial greeting is needed to let the communication partner know that the
  // data link is established (like handshake)
  Serial.println("Greetings!");

  // write initial led color
  DmxSimple.writeToBuffer(5, 0);
  DmxSimple.writeToBuffer(6, 0);
  DmxSimple.writeToBuffer(7, 170);
  DmxSimple.writeToBuffer(9, 170);
  DmxSimple.writeToBuffer(10, 0);
  DmxSimple.writeToBuffer(11, 0);
  DmxSimple.writeToBuffer(12, 170);
  DmxSimple.writeToBuffer(13, 170);
  DmxSimple.writeToBuffer(14, 146);
  DmxSimple.writeToBuffer(15, 100);
  DmxSimple.writeToBuffer(16, 170);
  DmxSimple.writeToBuffer(17, 170);
  DmxSimple.writeToBuffer(18, 0);
  DmxSimple.writeToBuffer(19, 0);
  DmxSimple.writeToBuffer(20, 170);
  DmxSimple.writeToBuffer(21, 170);
  DmxSimple.writeToBuffer(22, 0);
  DmxSimple.writeToBuffer(23, 0);
  DmxSimple.writeToBuffer(24, 170);
  DmxSimple.writeToBuffer(25, 170);
  DmxSimple.writeToBuffer(26, 0);
  DmxSimple.writeToBuffer(27, 0);
  DmxSimple.writeToBuffer(28, 170);
  DmxSimple.sendData();
}


void loop() {
  // wait for serial event
  if (!dataComplete)
    return;
  dataComplete = false;
  
  /*
    parse values and write to output
    for each character in data
      if char is number -> concatenated until whole number is read
      if char is ',' or '\n' -> write parsed number to channel because whole number was read
      else error
  */
  String curStr = "";
  char curChar = data[0];
  int curAddress = 1;
  bool error = false;
  for(int i = 0; i < strlen(data.c_str()); i++) {
    curChar = data[i];
    if(curChar >= '0' && curChar <= '9')
      curStr += curChar;
    else if(curChar == ',' || curChar == '\n') {
      // parse to int and write to output
      int value = atoi(curStr.c_str());

      if(value >= 0 && value < 256 && curStr != ""){
        DmxSimple.writeToBuffer(curAddress, value);
        if(PRINT_DEBUG_ENABLED) {
          Serial.print("wrote to output. value: ");
          Serial.print(value);
          Serial.print(", address: ");
          Serial.println(curAddress);
        }
        curStr = "";
        curAddress++;
      }
      else {
        Serial.print("could not parse data, invalid value: ");
        Serial.println(value);
        error = true;
        break;
      }
    } else {
      Serial.print("could not parse data, invalid character: ");
      Serial.write(curChar);
      Serial.println();
      error = true;
      break;
    }
  }

  // empty to receive new data
  data = "";

  if(!error) {
    DmxSimple.sendData();
    if(PRINT_DEBUG_ENABLED)
      Serial.println("sent data to output!");
  }
  
  if(PRINT_DEBUG_ENABLED)
    Serial.println();
}


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    data += inChar;
    if (inChar == '\n')
      dataComplete = true;
  }
}
