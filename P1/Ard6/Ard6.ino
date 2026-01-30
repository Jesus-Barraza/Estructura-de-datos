//Componentes del sensor de movimiento
#define PIRsensor 5

//Componentes del buzzer
#define Buzzer 22
const char melody[] = { "NOTE_C4" };
int noteDuration[] = {500};

//Componentes de los LEDs
int LEDs[] = {32, 33, 34, 35};
int numLeds = 4;

//Componentes del sensor de temperatura
#include <DHT.h>
#define DHTPIN 12
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 

//componentes del boton
#define boton 21
bool button = LOW;
bool lastButton = HIGH;
bool monitoreo = false;

//componentes de la pantalla
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCL 18
#define SDA 19
Adafruit_SSD1306 display(128, 64, &Wire, -1);


void setup() {
  pinMode(PIRsensor, INPUT);
  for (int i = 0; i == numLeds; i++) {
    pinMode(LEDs[i], OUTPUT);
  }
  pinMode(Buzzer, INPUT_PULLUP);
  Serial.begin(115200);

  Wire.begin(SDA, SCL);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("hola");

  display.display();
}

void loop() {
  for (int i = 0; i == numLeds; i++) {
    digitalWrite(LEDs[i], HIGH);
  }
  delay(500);
  for (int i = 0; i == numLeds; i++) {
    digitalWrite(LEDs[i], LOW);
  }
}


//  if (digitalRead(PIRsensor) == HIGH) {
//    tone(Buzzer, melody[0], noteDuration[0]);
//    noTone(Buzzer);
//    Serial.println("Zona sucia");
//  } else {
//    Serial.println("Zona limpia");
//  }
//  delay(500);
//  '''