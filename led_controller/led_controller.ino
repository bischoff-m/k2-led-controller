/**
 * TODO: rewrite doc
 * 
 * This program uses DmxSimple to contol LEDs at K2 Bar.
 * It sends multiple values v_i with 0 <= v_i <= 255 to the corresponding
 * channel i with 0 <= i < n (n is number of channels)
 * 
 * Communication:
 *  - Via Serial with Baudrate 115200
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
  Serial.begin(115200);
  while (!Serial) continue;

  // reserve 600 bytes for data
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

}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while(Serial.available()) {
    char inChar = (char)Serial.read();
    // TODO: remove 'X' case
    if(inChar == 'X') {
      Serial.println("Stopping");
      int count = 0;
      int left = Serial.available();
      while(Serial.available()) {
        Serial.read();
        count++;
      }
      Serial.print("Left: ");
      Serial.print(count);
      Serial.print("\t");
      Serial.println(left);
      while(true) ;
    } else if(inChar == '\n')
      parseAndSend();
    else
      data += inChar;
  }
}


/*
  parse values and write to output
  for each character in data
    if char is number -> concatenated until whole number is read
    if char is ',' or '\n' -> write parsed number to channel because whole number was read
    else error
*/
void parseAndSend() {
  if(data.length() % 2 != 0) {
    if(PRINT_DEBUG_ENABLED)
      Serial.println("Error: wrong length");
    data = "";
    return;
  }
  if(strspn(data.c_str(), "0123456789abcdefABCDEF") != data.length()) {
    if(PRINT_DEBUG_ENABLED)
      Serial.println("Error: non-hexadecimal value");
    data = "";
    return;
  }

  // TODO: remove case for better expandability
  if(data.length() != 56) {
    if(PRINT_DEBUG_ENABLED)
      Serial.println("Error: not 56 chars long");
    Serial.println(data);
    data = "";
    return;
  }

  int curAddress = 1;
  for(int i = 0; i < data.length(); i += 2) {
    char subStr[2] = {data[i], data[i + 1]};
    int value = (int)strtol(subStr, NULL, 16);
    DmxSimple.writeToBuffer(curAddress, value);
    curAddress++;
  }

  // empty to receive new data
  data = "";
  DmxSimple.sendData();
}