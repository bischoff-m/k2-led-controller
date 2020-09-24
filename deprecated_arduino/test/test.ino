int redPin = 9;
int greenPin = 10;
int bluePin = 11;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, HIGH);
}

void loop() {
  for(int helligkeit = 1; helligkeit <= 255; helligkeit++) {
    analogWrite(redPin, helligkeit);
    delay(1);
  }
  for(int helligkeit = 255; helligkeit >= 0; helligkeit--) {
    analogWrite(redPin, helligkeit);
    delay(1);
  }
}
