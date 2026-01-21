const int leds[] = {2,4,5};
const int numLeds = 3;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i],HIGH);
    delay(2000);
    digitalWrite(leds[i],LOW);
  }
}
