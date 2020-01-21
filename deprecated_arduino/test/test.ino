#include <DmxSimple.h>

void setup() {
  DmxSimple.usePin(3);
  int address = 1;
  for(int i = 0; i < 7; i++) {
    DmxSimple.write(address + 0, 100);
    DmxSimple.write(address + 1, 100);
    DmxSimple.write(address + 2, 100);
    DmxSimple.write(address + 3, 170);
    address += 4;
  }
}

void loop() {

}
