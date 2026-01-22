#include <DHT.h>
#define DHTPIN 23  //pin digital, sensor de señal
#define DHTTYPE DHT11 //modelo a usar lol

DHT dht(DHTPIN, DHTTYPE); //requisitos de la librería

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

//Status booleanos
bool stat1=LOW;
bool stat2=LOW;

int LEDs[] = {4, 18};
int numLed = 2;

int facTemp = 37;
int facHum = 14;

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < numLed; i++) {
    pinMode(LEDs[i], OUTPUT);
  }

  Serial.begin(9600);
  Serial.print("Sensor test");
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  const unsigned long interval1 = 250000/(temp*facTemp);
  const unsigned long interval2 = 250000/(hum*facHum);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;
    stat1 = !stat1;
    digitalWrite(LEDs[0], stat1);
    Serial.println("Temperature level: ");
    Serial.println(dht.readTemperature());
  }

  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    stat2 = !stat2;
    digitalWrite(LEDs[1], stat2);
    Serial.println("Humidity level: ");
    Serial.println(dht.readHumidity());
  }
}