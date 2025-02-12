/*
Manages the embedded LED on the card.

Yellow -> Robot booting up
Blue -> Waiting for a wifi client to connect
Green -> Wifi connected
*/

void SetupLed() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
} 


void SetLedColor(int red, int green, int blue) {

  analogWrite(LEDR, 255 - red);
  analogWrite(LEDG, 255 - green);
  analogWrite(LEDB, 255 - blue);

}